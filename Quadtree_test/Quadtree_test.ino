//=========================Includes==============================
#include <SPI.h>
#include <SD.h>
#include <avr/dtostrf.h>
#include <IridiumSBD.h>
//#include <IridiumSBD.h>
#include "Adafruit_FONA.h"
#include "wiring_private.h" //pinPeripheral
#include <quadtree.h>

//==========================Defines===============================
//Consulta de saldo Kolbi llamando al *888#
//FONA
#define FONA_RI 15
#define FONA_KEY 14
#define FONA_RST 10
#define FONA_PS 6
#define FONA_NS 7

//General
#define Serial SERIAL_PORT_USBVIRTUAL
#define SD_CS 16
#define ISBD_EN 10

//=========================SERCOM==============================

//Se instancian los puertos SPI y UART a partir de los SERCOM del SAMD21
//Ver https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports
SPIClass sdSPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
Uart Serial2 (&sercom2, 3, 4, SERCOM_RX_PAD_1, UART_TX_PAD_0);
Uart Serial3 (&sercom3, 7, 6, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial2.IrqHandler();
}

void SERCOM3_Handler()
{
  Serial3.IrqHandler();
}


//=====================Variables Globales========================
//ID del dispositivo
const unsigned int id = 12345u;

//Variables para el cálculo de distancia entre 2 puntos
float last_lat = 0;
float last_lon = 0;
float curr_lat, curr_lon;

//Variables auxiliares para cálculo de tiempo
long t1;
long t2 = 0;

//Estructura de datos para determinación de zona más cercana
Quadtree remora_gis = Quadtree();
vector<Data> test_points;


//------------------------------ CONFIGURABLES--------------------------------------
float los = 0.005f; 
uint16_t time_trig = 90;
uint8_t delay_sec = 10;
bool em_mode = 0;


//========================PENDIENTE======================
String net_apn = "";
String net_usr = "";
String net_pass = "";

String get_url = "";
String post_url = "";
//=======================================================


//===========================Setup================================

//Se asigna Serial2 como puerto de comunicación para FONA y se inicializa
HardwareSerial *fonaSerial = &Serial2;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

//Se inicializa Serial3 para RockBLOCK
IridiumSBD isbd(Serial3, ISBD_EN);

//Configuración RockBLOCK
void isbd_setup(){
  //Perfil de energía (1 para baja corriente)
  isbd.setPowerProfile(1);
}

//Configuración de FONA
void fona_setup() {
  pinMode(FONA_KEY, OUTPUT);
  pinMode(FONA_PS, INPUT);
  pinMode(FONA_NS, INPUT);
  digitalWrite(FONA_KEY, HIGH); //Se coloca el pin 15 en bajo para habilitar FONA (Key)
  while (!digitalRead(FONA_PS)) {
    digitalWrite(FONA_KEY, LOW);
    delay(2000);
    digitalWrite(FONA_KEY, HIGH);
    delay(2000);
    digitalWrite(FONA_KEY, LOW);
  }

  //Inicia solo si se logra establecer comunicación con FONA
  if (! fona.begin(*fonaSerial)) {
    while (1);
  }

  //Se habilita el GPS
  fona.enableGPS(true);
  fona.setGPRSNetworkSettings(F("kolbi3g"), F(""), F(""));
  Serial.println("FONA listo");
}

//Configuración de SD
void sd_setup() {
  //Se establece comunicación con la SD
  while (!SD.begin(SD_CS)) {
    delay(100);
  }
  Serial.println("SD lista");
}

//Configuración de puertos SERCOM (UART, SPI)
void sercom_setup() {
  //Se inicia comunicación UART para Serial2 y SPI para SD (DEBEN IR ANTES QUE pinPeripheral)
  fonaSerial->begin(4800);
  sdSPI.begin();
  Serial3.begin(19200);

  //Asigna funcionalidad SERCOM a pines 3(Rx) y 4(Tx)
  pinPeripheral(3, PIO_SERCOM_ALT);
  pinPeripheral(4, PIO_SERCOM_ALT);

   //Asigna funcionalidad SERCOM a pines 7(Rx) y 6(Tx)
  pinPeripheral(6, PIO_SERCOM_ALT);
  pinPeripheral(7, PIO_SERCOM_ALT);

  //Asigna funcionalidad SERCOM a pines 11(MOSI), 12(MISO) y 13(SCK)
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
  Serial.println("SERCOM listo");
}

//=========================Funciones==============================

//Consulta si el punto de prueba se encuentra en las áreas dadas
bool point_in_area(const vector<Data>& test_points, float lat, float lon)
{
  bool inside = false;
  for (auto i : test_points){
        inside = i.point_in_area(lat, lon);
        Serial.print("Colision con area ");
        Serial.print(i.pos.x);
        Serial.print(", ");
        Serial.print(i.pos.y);
        Serial.print(": ");
        Serial.println(inside);
        if (inside) break;
    }
    return inside;
}

//Escribir algún mensaje en un archivo en la tarjeta SD
boolean write_file(String file, String msg) {
  File file_out;
  file_out = SD.open(file, FILE_WRITE);
  //Escribir solo si el archivo se abrió adecuadamente
  if (file_out) {
    Serial.println("Escribiendo en SD");
    file_out.println(msg);
    file_out.close(); //Se cierra el archivo
    return true;
  } else {
    Serial.println("Error SD");
    return false;
  }
}

//Cargar las variables de configuración a memoria
void load_config_params() {
  String line;
  File cfg_file = SD.open("config.txt");
  cfg_file.readStringUntil('\n');
  if (cfg_file) {
    for(int i = 0; i < 4; i++){
      line = cfg_file.readStringUntil('\n');
      Serial.print("Línea: ");
      Serial.println(line);
      switch(i){
        case 0:
          los = line.toFloat();
          break;
        case 1:
          time_trig = (uint16_t)(line.toInt());
          break;
        case 2:
          delay_sec = (uint8_t)(line.toInt());
          break;
        case 3:
          em_mode = (uint8_t)(line.toInt());
          break;
      }
    }
  }
}

//Cargar las variables de red a memoria
void load_net_config() {
  String line;
  File net_file = SD.open("net.txt");
  net_file.readStringUntil('\n');
  if (net_file) {
    for(int i = 0; i < 4; i++){
      line = net_file.readStringUntil('\n');
      Serial.print("Línea: ");
      Serial.println(line);
      switch(i){
        case 0:
          net_apn = line;
          break;
        case 1:
          net_usr = line;
          break;
        case 2:
          net_pass = line;
          break;
      }
    }
  }
}

//Cargar las variables de servidor a memoria
void load_srv_config() {
  String line;
  File srv_file = SD.open("srv.txt");
  srv_file.readStringUntil('\n');
  if (srv_file) {
    for(int i = 0; i < 4; i++){
      line = srv_file.readStringUntil('\n');
      Serial.print("Línea: ");
      Serial.println(line);
      switch(i){
        case 0:
          get_url = line;
          break;
        case 1:
          post_url = line;
          break;
      }
    }
  }
}

//Cargar quadtree (mapa) a memoria
void load_map(){
  
}

//Consulta si la señal GSM es suficiente para transmisiones
bool test_gsm_rssi(){
  return (fona.getRSSI() >= 5) ? true : false;
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
      Serial.println("MAP");
      fona.sendSMS(sender, "MAP UPD"); //Intentar confirmar al remitente
      delay(100);
      //GET map
      load_map();
    }
    
    //Si el contenido del SMS es el código de emergencia
    if (replybuffer[0] == 'C') {
      Serial.println("SMS Config");
      fona.sendSMS(sender, "CFG ACK"); //Intentar confirmar al remitente
      delay(100);
      SD.remove("config.txt");
      write_file("config.txt", String(replybuffer));
      load_config_params();
    }

    //Si el contenido del SMS es el código de emergencia
    if (replybuffer[0] == 'N') {
      Serial.println("SMS Net");
      fona.sendSMS(sender, "NET ACK"); //Intentar confirmar al remitente
      delay(100);
      SD.remove("net.txt");
      write_file("net.txt", String(replybuffer));
      load_net_config();
    }

    //Si el contenido del SMS es el código de emergencia
    if (replybuffer[0] == 'S') {
      Serial.println("SMS Server");
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
  Serial.println("Calculando Ubicacion...");
  float speed_kph, heading, altitude;
  double date;
  boolean gps_success = fona.getGPS(&date, &curr_lat, &curr_lon, &speed_kph, &heading, &altitude);
  String msg = "[";
  if (gps_success) {
    msg = "{\"fecha\":" + String(date, 0) + ",\"lat\":" + String(curr_lat, 6) + ",\"lon\":" + String(curr_lon, 6) + ",\"vel\":" + speed_kph + ",\"alt\":" + altitude + "}";
    return msg;
  } else {
    Serial.println("Error GPS");
    return "ERROR";
  }
}

//Dado un mensaje con los datos del GPS (get_gps), se verifica si se debe escribir en la SD o no
void assert_write(String str) {
  if (str.indexOf("ERROR") == -1) {
    //Cálculo del tiempo transcurrido desde la última escritura
    int elapsed = (int)((t1 - t2) / 1000);
    Serial.print("Tiempo transcurrido: ");
    Serial.println(elapsed);
    if (( abs(curr_lat - last_lat) > los || abs(curr_lon - last_lon) > los) || elapsed > time_trig) {
      last_lat = curr_lat;
      last_lon = curr_lon;
      t2 = millis();
      write_file("log.txt", str);
      test_points = remora_gis.nearest_points(curr_lat, curr_lon, los);
    }
  }
}

//Realiza post al servidor
void fona_post(String url, String msg) {
  if (msg.indexOf("ERROR") == -1) {
    //Variablees necesarias para POST
    uint16_t statuscode;
    int16_t msg_len;
    char url_char[64];
    char data[128];
    
    url.toCharArray(url_char, 64);
    msg.toCharArray(data, 128);

    //POST del mensaje de ubicación al servidor
    uint8_t i = 0;
    bool gprs_en = fona.enableGPRS(true);
    while (!gprs_en && i < 5 ) {
      delay(300);
      gprs_en = fona.enableGPRS(true);
      i++;
    }
    delay(1000);
    fona.HTTP_POST_start(url_char, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&msg_len);
    fona.HTTP_POST_end();
    Serial.println(statuscode);
    if (statuscode == 200) {
      Serial.print("POST");
      Serial.println(msg);
    } else {
      Serial.println("Error de conexion");
    }
  }
}

//Envía señal de emergencia a través de red Iridium
void em_beacon(){
  //Enviar a servidor a través de rockBLOCK
  char coord[12];
  char str[24];
  dtostrf(curr_lat, 11, 6, coord);
  strcpy(str, coord);
  str[11] = ',';
  dtostrf(curr_lon, 11, 6, coord);
  strcat(str, coord);
  isbd.begin();
  Serial.println(str);
  /*uint8_t err = isbd.sendSBDText(str);
  if (err != 0)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
  }*/
  isbd.sleep();
  
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
        
  Serial.begin(115200);
  while (! Serial);
  Serial.println("======DEBUG======");
  sercom_setup();
  fona_setup();
  isbd_setup();
  sd_setup();

  //Cargar parámetros configurables
  load_config_params();
  load_net_config();
  load_srv_config();
  //load_map();

  //Se colocan los pines 25 y 26 (LEDs) en modo salida
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  
  remora_gis.insert(-84.110250f, 9.933845f, 0x1E1);
  remora_gis.insert(-84.108877f, 9.938325f, 0x2DF);
  remora_gis.insert(-84.098514f, 9.937848f, 0x2D4);
  remora_gis.insert(-84.098266f, 9.935712f, 0x2D2);
  remora_gis.insert(-84.099593f, 9.935712f, 0x2D6);
  remora_gis.insert(-84.099605f, 9.932733f, 0x2D2);

  remora_gis.insert(9.64f, 84.19f, 0x83);
  remora_gis.insert(39.10f, 18.99f, 0x47);

}

//========================CONSIDERAR CONDICIONES DE SEÑAL (RSSI)
void loop() {
  //Procesar SMS en cada ciclo de ejecución
  int8_t sms_num = fona.getNumSMS();
  if (sms_num > 0) {
    //Serial.println("INT_SMS");
    process_sms(sms_num);
  }
  
  //Obtener datos de ubicación
  String status_str = get_gps();

  //Modo de emergencia
  while(em_mode){
    em_beacon();
    delay(3600000); //Actualiza cada hora -- considerar sleep para el mcu
  }
  //Determinar si se ha roto algún umbral
  assert_write(status_str);
  
  if(point_in_area(test_points, curr_lat, curr_lon)){
    String msg = "{\"lat\":";
    msg += String(curr_lat);
    msg += ",\"lon\":";
    msg += String(curr_lon);
    msg += "}\n";
    write_file("violations.txt", msg);
    Serial.println("VIOLACION DE AREA");    
    //Encender alarma
  } else {
    em_mode = 0;
    //apagar bandera alarma
  }
  
  delay(delay_sec * 1000);
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
