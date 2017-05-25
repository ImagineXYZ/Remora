//=========================Includes==============================
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <avr/dtostrf.h>
#include "Adafruit_FONA.h"
#include <quadtree.h>
#include <ESP8266_XYZ.h>
#include "wiring_private.h" //pinPeripheral

#define Debug 1
#define DebugESP 0//Debug por ESP8266

//==========================Defines===============================
//Consulta de saldo Kolbi llamando al *888#
//FONA
#define FONA_RI 8
#define FONA_KEY 9
#define FONA_RST 18 //(A4)
#define FONA_NS 17 //(A3)

//General
#define Serial SERIAL_PORT_USBVIRTUAL//Puerto USB SerialUSB
#define SD_CS 15  //(A1)

//Red para debugging por ESP
#define SSID F("Javo")  //Nombre de la red a la que se desea conectar
#define PASS F("123456789")   //Contraseña de la red

//LED de prueba
#define LED 25

//=========================WDT==============================
const uint8_t periodoWDT = 11;

//=========================RAM Disponible==============================
//Rutina para calcular el espacio de memoria Ram disponible
extern "C" char *sbrk(int i);
int Ram () {
	char stack_dummy = 0;
	return &stack_dummy - sbrk(0);
}

//=========================SERCOM==============================
//Se instancian los puertos SPI y UART a partir de los SERCOM del SAMD21
//Ver https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports

//SPIClass sdSPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
Uart WSerial (&sercom1, 13, 11, SERCOM_RX_PAD_1, UART_TX_PAD_0);//Sercom1 multiuso

Uart Serial2 (&sercom2, 3, 4, SERCOM_RX_PAD_1, UART_TX_PAD_0);//Puerto para el FONA
//NOTA a cambiar: Se utiliza el sercom2_alt debería ser el sercom0 mismos pines

void SERCOM1_Handler(){WSerial.IrqHandler();}

void SERCOM2_Handler(){Serial2.IrqHandler();}

//=====================Variables Globales========================
//ID del dispositivo
String id = "XYZ001";

//Variables para el cálculo de distancia entre 2 puntos
float last_lat = 0;
float last_lon = 0;
float curr_lat, curr_lon;

//Variables auxiliares para cálculo de tiempo
long t1;
long t2 = 0;

//Auxiliares para el manejo de memoria durante la subida de datos
String line;//Variable para almacenar temporalmente los datos de la SD en memoria
signed long pos_a = 0;//Última posición en el archivo log.txt que se subió con respuesta del servidor 200

//Variables de conteo para reset del FONA
//GSM
const uint8_t TryGSM = 5;//Cantidad de intentos antes de reset general del FONA
uint8_t GSMCount = 0;
//GPS
const uint8_t TryGPS = 15;//Cantidad de intentos antes del on/off en GPS del FONA
uint8_t NoGPSCount = 0;
//GPRS
const uint8_t TryGPRS = 5;//Cantidad de intentos antes del on/off en GPRS

//Estructura de datos para determinación de zona más cercana
Quadtree remora_gis = Quadtree();
vector<Data> test_points;

//------------------------------ CONFIGURABLES POR SMS--------------------------------------
//Variables para parámetros umbrales
float los = 0.005f;
uint16_t time_trig = 90;
uint8_t delay_sec = 1;
bool em_mode = 0;//Por ahora no se utiliza

//Variables para parámetros de red celular
String net_apn = "";
String net_usr = "";
String net_pass = "";

//Variables para parámetros de servidor
String get_url = "";
String post_url = "";

//===========================Setup================================
//Se asigna Serial2 como puerto de comunicación para FONA y se inicializa
HardwareSerial *fonaSerial = &Serial2;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
//Puerto serial para debugging por ESP
ESP8266_XYZ esp(&WSerial);    //Arg #1: puntero al stream, Arg #2: pin para reset

//Configuración de FONA
void fona_setup() {
	pinMode(FONA_KEY, OUTPUT);
	pinMode(FONA_NS, INPUT);
	pinMode(FONA_RST, OUTPUT);
	digitalWrite(FONA_RST, HIGH); //Se coloca en alto para evitar que el FONA esté en RST permanente

	//Como no se tiene PowerStatus, se enciende ó se apaga el módulo para entrar en la rutina de anti_jam
	digitalWrite(FONA_KEY, LOW);
	delay(1000);
	digitalWrite(FONA_KEY, HIGH);//El pin debe quedar en alto para aseguar que el botón físico de ON/OFF del FONA funcione

	uint8_t FONAONOFF = Net_Status();
	if (FONAONOFF == 4){
		anti_jam(FONAONOFF);//lo enciende y lo registra
	}

	else{
		anti_jam(2);//realiza un reset y lo registra
	}
}

//Configuración de SD
void sd_setup() {
	//Se establece comunicación con la SD
	while (!SD.begin(SD_CS)) {
		delay(100);
	}
	if (Debug) {
		Serial.println(F("SD lista"));
	}
}

//Configuración de puertos SERCOM (UART, SPI)
void sercom_setup() {
	//Se inicia comunicación UART/SPI/I2C (DEBEN IR ANTES QUE pinPeripheral)
	fonaSerial->begin(4800);
	if (DebugESP){
		WSerial.begin(115200);

		//Asigna funcionalidad SERCOM1 a pines 13(Rx) y 11(Tx)
		pinPeripheral(13, PIO_SERCOM);
		pinPeripheral(11, PIO_SERCOM);

	}

	//Asigna funcionalidad SERCOM2 a pines 3(Rx) y 4(Tx)
	pinPeripheral(3, PIO_SERCOM_ALT);
	pinPeripheral(4, PIO_SERCOM_ALT);

	if (Debug) {
		Serial.println(F("SERCOM listo"));
	}
}

//Configuración del ESP para debugging
void esp_setup(){
	while (!esp.espTest());
	if(Debug){
		Serial.println("ESP Test");
	}
	while (!esp.softReset());
	if(Debug){
		Serial.println("ESP Reset");
	}

	while (!esp.connectAP(SSID, PASS)){
		delay(5000);
	}
	//no se encuentra en un ciclo, signifuca que puede o no conectarse y seguir con el programa

	if(Debug){
		Serial.println("ESP AP");
		Serial.println("Setup OK"); //Si se muestra existe conexión
	}
}

//=========================Funciones==============================//

//=========================Para la SD==============================//
//Escribir algún mensaje en un archivo en la tarjeta SD
boolean write_file(String file, String msg) {
	File file_out;
	file_out = SD.open(file, FILE_WRITE);
	//Escribir solo si el archivo se abrió adecuadamente
	if (file_out) {
		if (Debug) {
			Serial.print(F("Escribiendo en "));
			Serial.print(file);
			Serial.print(F(": "));
			Serial.println(msg);
		}
		if (DebugESP) {
			esp.print(msg);
		}
		file_out.println(msg);
		file_out.close(); //Se cierra el archivo
		return true;
	} else {
		if (Debug) {
			Serial.println(F("Error SD"));
		}
		if (DebugESP) {
			esp.println(F("Error SD"));
		}
		return false;
	}
}

//Lee un archivo de SD e imprime en pantalla
void Read_file (String file){
	File file_out;
	file_out = SD.open(file);
	if (file_out) {
		if (Debug){
			Serial.print(F("Reading "));
			Serial.println(file);
			while (file_out.available()) {
				Serial.write(file_out.read());
			}
		}
		file_out.close();
	}
	else {
		if (Debug){
			Serial.print(F("Error opening "));
			Serial.println(file);
		}
		if (DebugESP){
			esp.print(F("Error opening SD"));
		}
	}
}

//Trunca un archivo SD (0 bytes)
void Trunc_file(String file) {
	File file_out;
	file_out = SD.open(file, O_WRITE|O_TRUNC);
	file_out.close();
}

//Cargar las variables de configuración a memoria
void load_config_params() {
	File cfg_file = SD.open("config.txt");
	cfg_file.readStringUntil('\n');
	if (cfg_file) {
		for(int i = 0; i < 4; i++){
			line = cfg_file.readStringUntil('\n');
			if (Debug) {
				Serial.print(F("Config_Linea: "));
				line.trim();
				Serial.println(line);
			}
			//if (DebugESP){
			//esp.print(String(line));
			//}
			switch(i){
			case 0:
				line.trim();
				los = line.toFloat();
				break;
			case 1:
				line.trim();
				time_trig = (uint16_t)(line.toInt());
				break;
			case 2:
				line.trim();
				delay_sec = (uint8_t)(line.toInt());
				break;
			case 3:
				line.trim();
				em_mode = (uint8_t)(line.toInt());
				break;
			}
		}
		cfg_file.close();
	}
}

//Cargar las variables de red a memoria
//Agregar función para reasignar los parámetros de red celular (ON/OFF del FONA???)
void load_net_config() {
	File net_file = SD.open("net.txt");
	net_file.readStringUntil('\n');
	if (net_file) {
		for(int i = 0; i < 3; i++){
			line = net_file.readStringUntil('\n');
			if (Debug) {
				Serial.print(F("Net_Linea: "));
				line.trim();
				Serial.println(line);
			}
			//if (DebugESP){
			//esp.print(String(line));
			//}
			switch(i){
			case 0:
				line.trim();
				net_apn = line;
				break;
			case 1:
				line.trim();
				net_usr = line;
				break;
			case 2:
				line.trim();
				net_pass = line;
				break;
			}
		}
		net_file.close();
	}
}

//Cargar las variables de servidor a memoria
void load_srv_config() {
	File srv_file = SD.open("srv.txt");
	srv_file.readStringUntil('\n');
	if (srv_file) {
		for(int i = 0; i < 2; i++){
			line = srv_file.readStringUntil('\n');
			if (Debug) {
				Serial.print(F("Server_Linea: "));
				line.trim();
				Serial.println(line);
			}
			//if (DebugESP){
			//esp.print(String(line));
			//}
			switch(i){
			case 0:
				line.trim();
				get_url = line;
				break;
			case 1:
				line.trim();
				post_url = line;
				break;
			}
		}
		srv_file.close();
	}
}

//Retorna la última línea que se subió correctamente al server, lo lee de un archivo en la SD
unsigned long load_json_line(){
	File line_file = SD.open("jsonline.txt");
	line = line_file.readStringUntil('\n');

	if (line_file) {
		line.trim();
		signed long logline = line.toInt();
		line_file.close();
		return logline;
	}

	else {
		if (Debug){
			Serial.println(F("Error opening jsonline.txt"));
		}
		if (DebugESP){
			esp.println(F("Error opening jsonline.txt"));
		}
		return -1;
	}
}

//=========================Para el LED==============================//
//Funcion led para debugging
void ledbugging(int frecuencia, boolean onoff){
	for(int i = 0; i<=5;i++){
		digitalWrite(LED,HIGH);
		delay(50*frecuencia);
		digitalWrite(LED,LOW);
		delay(50*frecuencia);
	}
	if (onoff){
		digitalWrite(LED,HIGH);
	}
	else{
		digitalWrite(LED,LOW);
	}
}

//=========================Para el FONA==============================//
//Rutina para Encender FONA, reset FONA o Activar GPRS
void anti_jam(int typeofjamming){

	if (typeofjamming == 0){//estado pediente

	}

	else if (typeofjamming == 1){//GPRS activo
		//Todo bajo control
	}

	else if (typeofjamming == 2){//FONA sin registro en la red celular pero con servicio
		digitalWrite(FONA_RST, LOW);
		delay(1000);
		digitalWrite(FONA_RST, HIGH);
		delay (1000);
		if (!fona.begin(*fonaSerial)) {
			while (1) {
				if (Debug) {
					Serial.println(F("FONA no pudo iniciar"));
				};
			}
		}
		fona.enableGPS(true);
		fona.setGPRSNetworkSettings(F("kolbi3g"), F(""), F(""));
		if (Debug) {
			Serial.println(F("FONA listo"));
		}
		if (DebugESP) {
			esp.println(F("FONA listo"));
		}
	}

	else if (typeofjamming == 3){//Registrado en la red celular pero sin activar GPRS
		fona.enableGPRS(true);
		delay(300);
	}

	else if (typeofjamming == 4){//FONA apagado
		digitalWrite(FONA_KEY, LOW);
		delay(2000);
		digitalWrite(FONA_KEY, HIGH);
		delay(1000);
		if (!fona.begin(*fonaSerial)) {
			while (1) {
				if (Debug) {
					Serial.println(F("FONA no pudo iniciar"));
				};
			}
		}
		fona.enableGPS(true);
		fona.setGPRSNetworkSettings(F("kolbi3g"), F(""), F(""));

		if (Debug) {
			Serial.println(F("FONA listo"));
		}
		if (DebugESP) {
			esp.println(F("FONA listo"));
		}
	}
}

//Rutina para encender el GPS del FONA
void anti_jam_GPS(){
	//Se habilita el GPS
	fona.enableGPS(false);
	delay(3000);
	fona.enableGPS(true);

	if (Debug) {
		Serial.println(F("GPS reboot"));
	}
	if (DebugESP) {
		esp.println(F("GPS reboot"));
	}
}

//Rutina para detectar el estado del FONA a partir del pin NetStatus
//Requiere de un solo ciclo on-off-on del PIN para detectar el estado
int Net_Status(){
	long time0;
	long time1;
	bool WaitSignalHigh = true;
	bool SignalLow = true;
	bool SignalLowEnd = true;

	time0 = millis();
	while(WaitSignalHigh){
		time1 = millis();
		if(digitalRead(FONA_NS) == 1){
			while(SignalLow){
				if(digitalRead(FONA_NS) == 0){
					time0 = millis();
					while(SignalLowEnd){
						time1 = millis();
						if(digitalRead(FONA_NS) == 1){
							WaitSignalHigh = false;
							SignalLow = false;
							SignalLowEnd = false;
						}
						continue;
					}
				}
				continue;
			}
		}
		if ((time1-time0) > 3500){//detecta si el FONA se encuentra apagado
			WaitSignalHigh = false;
			SignalLow = false;
			SignalLowEnd = false;
		}
		continue;
	}

	if (Debug){
		Serial.print(time1-time0);
		Serial.print(F(" ms. "));
	}

	if((time1-time0) < 300){
		if (Debug){
			Serial.println(F("NS: GPRS"));
		}
		return 1;
	}
	else if ((time1-time0) < 1000){
		if (Debug){
			Serial.println(F("NS: No Net"));
		}
		return 2;
	}
	else if ((time1-time0) < 3000){
		if (Debug){
			Serial.println(F("NS: GSM"));
		}
		return 3;
	}

	else if ((time1-time0) > 3500){
		if (Debug){
			Serial.println(F("NS: FONA Off"));
		}
		return 4;
	}
	else{
		return 0;
	}
}

//Procesamiento de SMS para configuración de variables de sistema
void process_sms(int8_t sms_num) {
	uint8_t n = 1;
	//Se leen todos los SMS con el fin de verificar códigos dentro de ellos
	while (n <= sms_num) {

		//Buffers y variable de longitud
		uint16_t smslen;
		char sender[32];
		char replybuffer[255];

		//Lectura de un SMS
		uint8_t len = fona.readSMS(n, replybuffer, 250, &smslen);

		//Si len = 0, se debe buscar otro SMS de memoria
		if (len == 0) {
			n++;
			continue;
		}

		//Se intenta obtener el número del remitente del mensaje
		if (! fona.getSMSSender(n, sender, sizeof(sender))) {
			//No se logra obtener (número privado)
			sender[0] = 0;
		}

		if (replybuffer[0] == 'M') {
			if (Debug) {
				Serial.println(F("MAP"));
			}
			fona.sendSMS(sender, "MAP UPD"); //Intentar confirmar al remitente
			delay(100);
			//GET map
			//load_map();
		}

		//Si el contenido del SMS es el código de emergencia
		if (replybuffer[0] == 'C') {
			if (Debug) {
				Serial.println(F("SMS Config"));
			}
			fona.sendSMS(sender, "CFG ACK"); //Intentar confirmar al remitente
			delay(100);
			SD.remove("config.txt");
			write_file("config.txt", String(replybuffer));
			load_config_params();
		}

		//Si el contenido del SMS es el código de emergencia
		if (replybuffer[0] == 'N') {
			if (Debug) {
				Serial.println(F("SMS Net"));
			}
			fona.sendSMS(sender, "NET ACK"); //Intentar confirmar al remitente
			delay(100);
			SD.remove("net.txt");
			write_file("net.txt", String(replybuffer));
			load_net_config();
		}

		//Si el contenido del SMS es el código de emergencia
		if (replybuffer[0] == 'S') {
			if (Debug) {
				Serial.println(F("SMS Server"));
			}
			fona.sendSMS(sender, "SRV ACK"); //Intentar confirmar al remitente
			delay(100);
			SD.remove("srv.txt");
			write_file("srv.txt", String(replybuffer));
			load_srv_config();
		}

		delay(200);
		fona.deleteSMS(n); //Se borra todo mensaje leído
		break;
	}
}

//Se obtiene una cadena con fecha, latitud, longitud, velocidad, orientación y altitud del dispositivo
String get_gps() {
	if (Debug) {
		Serial.println(F("Calculando Ubicacion..."));
	}
	float speed_kph, heading, altitude,hdop,pdop,vdop,hpa,vpa;
	double date;
	bool run,fix;
	uint8_t fix_mode,gps_view,gns_used;
	float fuel = 1200.00;
	int Motor = 1;

	boolean gps_success = fona.getGPS(&date, &curr_lat, &curr_lon, &speed_kph, &heading, &altitude,&run,&fix, &fix_mode,&hdop, &pdop, &vdop, &gps_view, &gns_used, &hpa, &vpa);
	String msg = "";
	if (gps_success) {
		ledbugging(1,1);
		//Todos los datos del GPS
		msg = "{\"ID\":\"" + id + "\",\"fecha\":" + String(date, 0) + ",\"lat\":" + String(curr_lat, 6) + ",\"lon\":" + String(curr_lon, 6) + ",\"vel\":" + String(speed_kph,2) + ",\"alt\":" + String(altitude,3) + ",\"Head\":" + String(heading,2) + ",\"RunStatus\":" +  String(run) + ",\"Fix\":" +  String(fix) + ",\"FixMode\":" +  String(fix_mode) + ",\"HDOP\":" +  String(hdop,2) + ",\"PDOP\":" +  String(pdop,2)+ ",\"VDOP\":" +  String(vdop,2) + ",\"GPSView\":" +  String(gps_view) + ",\"GNSS_used\":" +  String(gns_used) + ",\"HPA\":" +  String(hpa,1)  + ",\"VPA\":" +  String(vpa,1) + ",\"fuel\":" + String(fuel) + ",\"Motor\":" + String(Motor) + ",\"QuadTree\":" + String(point_in_area(test_points, curr_lat, curr_lon));
		NoGPSCount = 0;
		return msg;
	}

	else {
		ledbugging(1,0);
		if (Debug) {
			Serial.println(F("Error GPS"));
		}
		NoGPSCount++;
		if (NoGPSCount >= TryGPS){
			if (Debug) {
				Serial.println(F("Anti Jamming for GPS"));
			}
			NoGPSCount = 0;
			anti_jam_GPS();
		}
		return "ERROR";
	}
}

//Realiza post al servidor
int fona_post(String url, String msg) {

	//Variablees necesarias para POST
	uint16_t statuscode;
	int16_t msg_len;

	if (msg.indexOf("ERROR") == -1) {


		char url_char[46]; //length +1
		char data[300];

		//{"ID":XYZ001,"fecha":20170523230402,"lat":99.937687,"lon":-144.061256,"vel":100.09,"alt":10209.80,"Head":258.50,"RunStatus":1,"Fix":1,"FixMode":1,"HDOP":10.20,"PDOP":10.50,"VDOP":10.90,"GPSView":12,"GNSS_used":15,"HPA":39.0,"VPA":10.0,"fuel":0000.00,"Motor":1,"QuadTree":0,"RSSI":"18"}		//{"ID":"XYZ001","fecha":"20160914234855","lat":"-90.937282","lon":"-180.059265","vel":"999.99","alt":"10018.9","Head":"279.10","RunStatus":"1","Fix":"1","FixMode":"1","HDOP":"10.2","PDOP":"10.8","VDOP":"10.9","GPSView":"99","GNSS_used":"99","GNSS_view":"99","HPA":"9999.9","VPA":"9999.9","fuel":"0000.00","Motor":"1","Quadtree":"1","RSSI":"18"}
		url.toCharArray(url_char, 46); //length +1
		msg.toCharArray(data, 300);

		uint8_t i = 0;
		uint8_t estado = Net_Status();

		while(estado != 1){

			//que hacer si no hay red, estado == 2
			if (estado == 3){

				if (Debug){
					Serial.println(F("Activando GPRS..."));
				}
				anti_jam(estado);
				i++;
			}
			if (i > TryGPRS){
				return -1;
			}
			estado = Net_Status();
		}
		delay(1000);
		fona.HTTP_POST_start(url_char, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&msg_len);
		fona.HTTP_POST_end();
		if (Debug) {
			Serial.println(statuscode);
		}

		if (statuscode == 200) {
			if (Debug) {
				Serial.print(F("POST: "));
				Serial.println(msg);
			}
			write_file("updat.txt", msg);		//Escritura en SD
		}

		else {
			if (Debug) {
				Serial.print(F("Error de conexion: "));
				Serial.println(msg);
			}
			msg = msg + "   Errror code: " + String(statuscode);
			write_file("nonupdat.txt", msg);		//Escritura en SD
		}
		return statuscode;
	}
	statuscode = 0;
	return statuscode;
}

//=========================Para REMORA==============================//
//Dado un mensaje con los datos del GPS (get_gps), se verifican los umbrales, escribe en el logger
void assert_write(String str) {
	if (str.indexOf("ERROR") == -1) {
		ledbugging(3,1);
		//Cálculo del tiempo transcurrido desde la última escritura
		int elapsed = (int)((t1 - t2) / 1000);
		if (Debug) {
			Serial.println(F("--Analizando Umbrales--"));
			Serial.print(F("Tiempo transcurrido: "));
			Serial.println(elapsed);
			Serial.print(F("Distancia recorrida: "));
			Serial.print(F("Latitud: "));
			Serial.print(abs(curr_lat - last_lat));
			Serial.print(F(" Longitud: "));
			Serial.println(abs(curr_lon - last_lon));
		}
		if (DebugESP) {//Análisis de umbrales
			esp.println(String(elapsed) + " " + String(abs(curr_lat - last_lat)) + " " + String(abs(curr_lon - last_lon)));
		}
		if (( abs(curr_lat - last_lat) > los || abs(curr_lon - last_lon) > los) || elapsed > time_trig) {
			last_lat = curr_lat;
			last_lon = curr_lon;
			t2 = millis();
			write_file("log.txt", str);
			Service_available();

			/*long t3 = millis();
			test_points = remora_gis.nearest_points(curr_lat, curr_lon, los);
			long t4 = millis();
			Serial.print("Tiempo ejec. pts cercanos: ");
			Serial.println(t4-t3);*/
		}
	}
	else{
		ledbugging(3,0);
	}
}

//Analiza si hay servicio disponible para enviar datos a la nube
void Service_available(){
	uint8_t NetStatus = Net_Status();
	uint8_t SignalStatus = fona.getRSSI();

	//Consulta si la señal GSM es suficiente para transmisiones
	if (Debug){
		Serial.print(F("Service available: "));
		Serial.println(SignalStatus);
	}

	if ((SignalStatus > 10) && (NetStatus == 2)){
		if (Debug){
			Serial.println(F("Anti Jamming GSM with Service"));
			anti_jam(NetStatus);//antijamming para gsm: Me preocupa que entre muy rápido a esta rutina y nunca se registre en la red por causa del reset constante
		}
	}

	else if ((SignalStatus > 5) && (( NetStatus == 1) || ((NetStatus == 3)))){
		//Procesar SMS en cada ciclo de ejecución
		int8_t sms_num = fona.getNumSMS();
		if (sms_num > 0) {
			process_sms(sms_num);
		}

		//Load de la última posición del logger que se subió correctamente
		pos_a = load_json_line();

		if (pos_a == -1){
			Serial.print(F("Error leyendo pos_a"));
		}
		else{
			upload_data();
		}
	}
}

//Logica para enviar línea por línea a la nube... improvement: enviar paquetes
void upload_data(){

	String json;
	File log_file = SD.open("log.txt");
	if (!log_file)
	{
		if (Debug){
			Serial.println(F("Log.txt not found"));
		}
	}

	else{
		signed long file_size = log_file.size();
		if (Debug){
			Serial.print(F("Size: "));
			Serial.println(file_size);
			Serial.print(F("pos_a: "));
			Serial.println(pos_a);
		}

		if (file_size == 0){
			if (Debug){
				Serial.println(F("Log vacio"));
			}
		}
		else {

			log_file.seek(pos_a);
			json = log_file.readStringUntil('\n');

			signed long temp = log_file.position();
			log_file.close();

			json.trim();
			json = json + ",\"RSSI\":"+ String(fona.getRSSI())+"}";
			if (Debug){
				Serial.println(json);
			}

			if (( fona_post(post_url, json)) == 200){
				ledbugging(2,1);
				if (Debug){
					Serial.println(F("Nueva posicion en cursor"));
				}
				pos_a = temp;
				Trunc_file("jsonline.txt");
				write_file("jsonline.txt",String(pos_a));

				if (file_size == pos_a){
					if (Debug){
						Serial.println(F("Fin del archivo. Trunc log..."));
					}
					Trunc_file("jsonline.txt");
					write_file("jsonline.txt","0");
					Trunc_file("log.txt");
				}
				else{
					GSMCount = TryGSM;//Se coloca éste umbral para evitar una recursión y para que el próximo ciclo del loop llame a upload inmediatamente
				}
			}

			else{
				ledbugging(2,0);
				if (Debug){
					Serial.println(F("Error en la subida de datos"));
				}
			}
		}

	}
}

//=========================Para el QUADTREE==============================//
//Consulta si el punto de prueba se encuentra en las áreas dadas
bool point_in_area(const vector<Data>& test_points, float lat, float lon)
{
	bool inside = false;
	for (auto i : test_points){
		inside = i.point_in_area(lat, lon);
		if (Debug) {
			Serial.print(F("Colision con area "));
			Serial.print(i.pos.x);
			Serial.print(F(", "));
			Serial.print(i.pos.y);
			Serial.print(F(": "));
			Serial.println(inside);
		}
		if (inside) break;
	}
	return inside;
}

//Cargar quadtree (mapa) a memoria
void load_map(){

}

//=========================WDT==============================//

//Sincronización del WDT
static void WDTsync() {
	while (WDT->STATUS.bit.SYNCBUSY == 1);
}

//Sincronización del GCLK
static void GCLKsync() {
	while (GCLK->STATUS.bit.SYNCBUSY == 1);
}

// reset del WDT (DEBE ser con 0xA5)
void resetWDT() {
	WDT->CLEAR.reg = 0xA5;
	WDTsync();
}

// reset del sistema (DIFERENTE de 0xA5)
void systemReset() {
	WDT->CLEAR.reg = 0x00;
	WDTsync();
}

// desacttivar WDT
void disableWDT(){
	WDT->CTRL.reg = 0;
	WDTsync();
}

//Setup WDT
void setupWDT(uint8_t period) {
	WDT->CTRL.reg = 0; // Se desactiva WDT
	WDTsync(); // Sincronizar para no perder configuración

	GCLK->GENDIV.reg = 0x1F02; //Establece la división del generador de relojes (GCLKGEN2/2^5)
	GCLKsync(); // Sincronizar para no perder configuración

	GCLK->GENCTRL.reg = 0x110302; //Relaciona OSCULP32K con GCLKGEN2
	GCLKsync(); // Sincronizar para no perder configuración

	GCLK->CLKCTRL.reg = 0x4203; //Relaciona GCLKGEN2 con GCLK_WDT
	GCLKsync(); // Sincronizar para no perder configuración

	WDT->CONFIG.reg = min(period, periodoWDT); // Tabla 17-5 Timeout Period

	WDT->CTRL.reg = WDT_CTRL_ENABLE; //Activar WDT
	WDTsync(); // Sincronizar para no perder configuración
}

//===========================================================================================//
void setup() {
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	delay(1000);
	digitalWrite(LED, LOW);
	delay(1000);

	if (Debug){
		Serial.begin(9600);
		while (! Serial);
	}

	if (Debug) {
		Serial.println(F("======DEBUG======"));
	}

	sercom_setup();
	if (DebugESP){
		esp_setup();
	}

	fona_setup();
	sd_setup();

	if(Debug){
		Read_file("log.txt");
		Read_file("nonupdat.txt");
		Read_file("updat.txt");
	}

	//Trunc_file("log.txt");
	//Trunc_file("nonupdat.txt");
	//Trunc_file("updat.txt");

	int8_t sms_num = fona.getNumSMS();
	if (sms_num > 0) {
		process_sms(sms_num);
	}

	//Cargar parámetros configurables
	load_config_params();
	load_net_config();
	load_srv_config();
	//load_map();

	if (Debug) {
		Serial.println(F("======SETUP LISTO======"));
	}

	if (DebugESP) {
		esp.println(F("======SETUP LISTO======"));
	}

	//La Sabana
	//remora_gis.insert(-84.110250f, 9.933845f, 0x1E1);
	//remora_gis.insert(-84.108877f, 9.938325f, 0x2DF);
	//remora_gis.insert(-84.098514f, 9.937848f, 0x2D4);
	//remora_gis.insert(-84.098266f, 9.935712f, 0x2D2);
	//remora_gis.insert(-84.099593f, 9.935712f, 0x2D6);
	//remora_gis.insert(-84.099605f, 9.932733f, 0x2D2);

	//Office
	remora_gis.insert(-84.061049f,9.937767f,0x1F);

	//WDT
	setupWDT( periodoWDT ); //Inicialización del WDT
}

void loop() {
	resetWDT();
	/*if (Debug) {
		Serial.print("freeMemory()=");
		Serial.println(Ram());
	}

	if (DebugESP) {
		esp.print("freeMemory()=");
		esp.println(String(Ram()));
	}*/
	t1 = millis();//agregar t1 para tomar datos de tiempo y romper umbral

	//Obtener datos de ubicación
	String status_str = get_gps();

	if(point_in_area(test_points, curr_lat, curr_lon)){
		if (Debug) {
			Serial.print("Violacion");
		}
		if (DebugESP) {
			esp.print("Violacion");
		}
	}
	else {
		if (Debug) {
			Serial.print("Bajo Control");
		}
		if (DebugESP) {
			esp.print("Bajo Control");
		}
	}

	/*if (status_str != "ERROR") {
		assert_write(status_str); //Determinar si se ha roto algún umbral
	}*/

	//Rutina para realizar un reset al FONA en caso de que tenga señal y no esté registrado en la red
	GSMCount++;
	if (GSMCount >= TryGSM){
		GSMCount = 0;
		Service_available();//No solo anti jam para gsm sino que si hay señal pero no hay GPS sube los datos
	}

	else if (Debug){
		int cuenta = 0;
		while (cuenta <= delay_sec){
			Serial.print(F("."));
			delay(1000);
			cuenta++;
		}
		Serial.println(F("."));
	}
	else{
		delay(delay_sec * 1000);
	}
}


/*=======================TODO========================
 * Condiciones para GET y POST
 * Verificación de cumplimiento de embarcaciones
 *
 * --------------------------------------------------
 *
 * Remora solicita mapas, servidor responde cantidad de archivos
 * Mapa completo se divide en archivos que contienen puntos unicamente
 * Servidor retorna n archivos con su origen
 * Remora busca en archivos secuencialmente, server debería garantizar tamaño manejable
 *
 *
 */
