/*
filename: Comms.cpp
author: Sergio Vargas
date last modified: 05/01/2018
platform: ARM M0 (SAMD21)
Remora communications module source file.
*/

#include "Comms.h"

Comms::Comms(Persistence persistence, upload_t function){
	storage = &persistence;
	fonaSerial = &Serial2;
	uploadData = function;

	//Se inicia comunicación UART/SPI/I2C (DEBEN IR ANTES QUE pinPeripheral)
	fonaSerial->begin(4800);

	//Asigna funcionalidad SERCOM2 a pines 3(Rx) y 4(Tx)
	pinPeripheral(3, PIO_SERCOM_ALT);
	pinPeripheral(4, PIO_SERCOM_ALT);

	pinMode(FONA_KEY, OUTPUT);
	pinMode(FONA_NS, INPUT);
	pinMode(FONA_RST, OUTPUT);
	digitalWrite(FONA_RST, HIGH); //Se coloca en alto para evitar que el FONA esté en RST permanente

	//Como no se tiene PowerStatus, se enciende ó se apaga el módulo para entrar en la rutina de anti_jam
	digitalWrite(FONA_KEY, LOW);
	delay(1000);
	digitalWrite(FONA_KEY, HIGH); //El pin debe quedar en alto para aseguar que el botón físico de ON/OFF del FONA funcione

	uint8_t fonaStatus = netStatus();
	if (fonaStatus == 4)
	{
		setFonaStatus(fonaStatus); //lo enciende y lo registra
	}
	else
	{
		setFonaStatus(2); //realiza un reset y lo registra
	}
}

//Procesamiento de SMS para configuración de variables de sistema
void Comms::checkSms()
{
	int8_t currentSmsAmount = fona.getNumSMS();
	if (currentSmsAmount <= 0)
	{
		return;
	}
	uint8_t n = 1;
	//Se leen todos los SMS con el fin de verificar códigos dentro de ellos
	while (n <= currentSmsAmount)
	{
		//Buffers y variable de longitud
		uint16_t smsLen;
		char sender[32];
		char replyBuffer[255];

		//Lectura de un SMS
		uint8_t len = fona.readSMS(n, replyBuffer, 250, &smsLen);

		//Si len = 0, se debe buscar otro SMS de memoria
		if (len == 0)
		{
			n++;
			continue;
		}

		//Se intenta obtener el número del remitente del mensaje
		if (!fona.getSMSSender(n, sender, sizeof(sender)))
		{
			//No se logra obtener (número privado)
			sender[0] = 0;
		}

		//Si el SMS inicia con MAP
		//if (replyBuffer[0] == 'M')
		if (strcasecmp(replyBuffer, "MAP") == 0)
		{
			DEBUG_PRINT_LN(F("MAP"));
			fona.sendSMS(sender, (char *) "MAP UPD"); //Intentar confirmar al remitente
			delay(100);
			//GET map
			//load_map();
		}

		//Si el SMS inicia con CFG
		//if (replyBuffer[0] == 'C')
		if (strcasecmp(replyBuffer, "CFG") == 0)
		{
			DEBUG_PRINT_LN(F("SMS Config"));
			fona.sendSMS(sender, (char *) "CFG ACK"); //Intentar confirmar al remitente
			delay(100);
			storage->removeFile(storage->deviceConfigFile);
			storage->writeFile(storage->deviceConfigFile, String(replyBuffer));
			storage->loadDeviceConfig();
		}
		//Si el SMS inicia con NET
		//if (replyBuffer[0] == 'N')
		if (strcasecmp(replyBuffer, "NET") == 0)
		{
			DEBUG_PRINT_LN(F("SMS Net"));
			fona.sendSMS(sender, (char *) "NET ACK"); //Intentar confirmar al remitente
			delay(100);
			storage->removeFile(storage->netConfigFile);
			storage->writeFile(storage->netConfigFile, String(replyBuffer));
			storage->loadNetConfig();
		}

		//Si el SMS inicia con SRV
		//if (replyBuffer[0] == 'S')
		if (strcasecmp(replyBuffer, "SRV") == 0)
		{
			DEBUG_PRINT_LN(F("SMS Server"));
			fona.sendSMS(sender, (char *) "SRV ACK"); //Intentar confirmar al remitente
			delay(100);
			storage->removeFile(storage->serverConfigFile);
			storage->writeFile(storage->serverConfigFile, String(replyBuffer));
			storage->loadServerConfig();
		}

		delay(200);
		fona.deleteSMS(n); //Se borra todo mensaje leído
		break;
	}
}

//Se obtiene una cadena con fecha, latitud, longitud, velocidad, orientación y altitud del dispositivo
String Comms::getNmea(Nmea *data)
{
	DEBUG_PRINT_LN(F("Calculando Ubicacion..."));
	//{"ID":"XYZ001","fecha":20160914234855,"lat":-90.937282,"lon":-180.059265,"vel":999.99,"alt":10018.900,"Head":279.10,"RunStatus":1,"Fix":1,"FixMode":1,"HDOP":10.2,"PDOP":10.8,"VDOP":10.9,"GPSView":99,"GNSS_used":99,"HPA":9999.9,"VPA":9999.9
	boolean gpsSuccess = fona.getGPS(&data->date, &data->lat, &data->lon, &data->speedKph,
			&data->heading, &data->altitude, &data->run, &data->fix, &data->fixMode, &data->hdop,
			&data->pdop, &data->vdop, &data->gpsView, &data->gnsUsed, &data->hpa, &data->vpa);
	String msg = "";
	if (gpsSuccess)
	{
		// ------------Reemplazar mensaje JSON por struct Nmea------------
		//ledbugging(1, 1);
		//Todos los datos del GPS
		msg = "{\"ID\":\"" + deviceId + "\",\"fecha\":" + String(data->date, 0)
				+ ",\"lat\":" + String(data->lat, 6) + ",\"lon\":"
				+ String(data->lon, 6) + ",\"vel\":" + String(data->speedKph, 2)
				+ ",\"alt\":" + String(data->altitude, 3) + ",\"Head\":"
				+ String(data->heading, 2) + ",\"RunStatus\":" + String(data->run)
				+ ",\"Fix\":" + String(data->fix) + ",\"FixMode\":" + String(data->fixMode)
				+ ",\"HDOP\":" + String(data->hdop, 2) + ",\"PDOP\":"
				+ String(data->pdop, 2) + ",\"VDOP\":" + String(data->vdop, 2)
				+ ",\"GPSView\":" + String(data->gpsView) + ",\"GNSS_used\":"
				+ String(data->gnsUsed) + ",\"HPA\":" + String(data->hpa, 1) + ",\"VPA\":"
				+ String(data->vpa, 1);
		DEBUG_PRINT_LN(msg);
		//DEBUG_ESP_LN(msg);
		noGpsCount = 0;
		return msg;
	}
	else
	{
		//ledbugging(1, 0);
		DEBUG_PRINT_LN(F("Error GPS"));
		noGpsCount++;
		if (noGpsCount >= tryGps)
		{
			DEBUG_PRINT_LN(F("Anti Jamming for GPS"));
			noGpsCount = 0;
			rebootGps();
		}
		return "ERROR";
	}
}

//Rutina para encender el GPS del FONA
void Comms::rebootGps()
{
	//Se habilita el GPS
	fona.enableGPS(false);
	delay(3000);
	fona.enableGPS(true);
	DEBUG_PRINT_LN(F("GPS Reboot"));DEBUG_ESP_LN(F("GPS Reboot"));
}

//Rutina para Encender FONA, reset FONA o Activar GPRS
void Comms::setFonaStatus(int option)
{
	switch(option){
	case 0:
		break;
	case 1:
		break;
	case 2:
		digitalWrite(FONA_RST, LOW);
		delay(1000);
		digitalWrite(FONA_RST, HIGH);
		delay(1000);
		if (!fona.begin(*fonaSerial))
		{
			DEBUG_PRINT_LN(F("FONA no pudo iniciar"));
			while (true)
				;
		}
		fona.enableGPS(true);
		fona.setGPRSNetworkSettings(F("kolbi3g"), F(""), F(""));
		break;
	case 3:
		fona.enableGPRS(true);
		delay(300);
		break;
	case 4:
		digitalWrite(FONA_KEY, LOW);
		delay(2000);
		digitalWrite(FONA_KEY, HIGH);
		delay(1000);
		if (!fona.begin(*fonaSerial))
		{
			DEBUG_PRINT_LN(F("FONA no pudo iniciar"));
			while (true)
				;
		}
		fona.enableGPS(true);
		//Modificar FONA para no usar Flash String
		fona.setGPRSNetworkSettings(F("kolbi3g"), F(""), F(""));
		DEBUG_PRINT_LN(F("FONA listo"));DEBUG_ESP_LN(F("FONA listo"));
		break;
	}
}

//Rutina para detectar el estado del FONA a partir del pin NetStatus
//Requiere de un solo ciclo on-off-on del PIN para detectar el estado
int Comms::netStatus()
{
	long time0;
	long time1;
	bool waitSignalHigh = true;
	bool signalLow = true;
	bool signalLowEnd = true;

	time0 = millis();
	while (waitSignalHigh)
	{
		time1 = millis();
		if (digitalRead(FONA_NS) == 1)
		{
			while (signalLow)
			{
				if (digitalRead(FONA_NS) == 0)
				{
					time0 = millis();
					while (signalLowEnd)
					{
						time1 = millis();
						if (digitalRead(FONA_NS) == 1)
						{
							waitSignalHigh = false;
							signalLow = false;
							signalLowEnd = false;
						}
						continue;
					}
				}
				continue;
			}
		}
		if ((time1 - time0) > 3500)
		{		//detecta si el FONA se encuentra apagado
			waitSignalHigh = false;
			signalLow = false;
			signalLowEnd = false;
		}
		continue;
	}
	DEBUG_PRINTT(time1 - time0);
	DEBUG_PRINTT(F(" ms. "));

	if ((time1 - time0) < 300)
	{
		DEBUG_PRINT_LN(F("NS: GPRS"));
		return 1;
	}
	else if ((time1 - time0) < 1000)
	{
		DEBUG_PRINT_LN(F("NS: No Net"));
		return 2;
	}
	else if ((time1 - time0) < 3000)
	{
		DEBUG_PRINT_LN(F("NS: GSM"));
		return 3;
	}
	else if ((time1 - time0) > 3500)
	{
		DEBUG_PRINT_LN(F("NS: FONA Off"));
		return 4;
	}
	else
	{
		return 0;
	}
}

//Realiza post al servidor
int Comms::post(String url, String msg)
{

	//Variables necesarias para POST
	uint16_t statusCode;
	int16_t msgLen;

	if (msg.indexOf("ERROR") == -1)
	{
		//clearchar();

		char urlChar[46] = ""; //length +1 //direccion del POST
		char data[380] = ""; //Espacio reservado para el JSON

//		DEBUG_RAM("ChartoArray");
		//{"ID":"XYZ001","fecha":20160914234855,"lat":-90.937282,"lon":-180.059265,"vel":999.99,"alt":10018.900,"Head":279.10,"RunStatus":1,"Fix":1,"FixMode":1,"HDOP":10.2,"PDOP":10.8,"VDOP":10.9,"GPSView":99,"GNSS_used":99,"HPA":9999.9,"VPA":9999.9,"fuel":0000.00,"Motor":1,"Quadtree":1,"RSSI":18,"RAM":30000}
		url.toCharArray(urlChar, 46); //length +1
//		DEBUG_RAM("ChartoArray1");
		msg.toCharArray(data, 380);
//		DEBUG_RAM("ChartoArray2");
		uint8_t i = 0;
		uint8_t status = netStatus();

		while (status != 1)
		{
			//que hacer si no hay red, estado == 2
			if (status == 3)
			{
				DEBUG_PRINT_LN(F("Activando GPRS..."));
				setFonaStatus(status);
				i++;
			}
			if (i > tryGprs)
			{
				return -1;
			}
			status = netStatus();
		}
		delay(1000);

//		DEBUG_RAM("Pre-post");
		fona.HTTP_POST_start(urlChar, F("application/json"), (uint8_t *) data,
				strlen(data), &statusCode, (uint16_t *) &msgLen);
		fona.HTTP_POST_end();
//		DEBUG_RAM("Posted");
		DEBUG_PRINT_LN(statusCode);

		if (statusCode == 200)
		{
			DEBUG_PRINTT(F("POST: "));
			DEBUG_PRINT_LN(msg);
			storage->writeFile("updat.txt", msg);		//Escritura en SD
//			DEBUG_RAM("Saveupadat");
		}

		else
		{
			DEBUG_PRINTT(F("Error de conexion: "));
			DEBUG_PRINT_LN(msg);
			msg = msg + "   Errror code: " + String(statusCode);
			storage->writeFile("nonupdat.txt", msg);		//Escritura en SD
//			DEBUG_RAM("Savenonupadat");
		}
		return statusCode;
	}
	statusCode = 0;
	return statusCode;
}

//Analiza si hay servicio disponible para enviar datos a la nube
void Comms::uploadIfServiceAvailable()
{
	uint8_t mNetStatus = netStatus();
	//DEBUG_RAM("NETStatus", Ram());
	uint8_t mSignalStatus = fona.getRSSI();
	//Consulta si la señal GSM es suficiente para transmisiones
	DEBUG_PRINTT(F("Service available: "));
	DEBUG_PRINT_LN(mSignalStatus);

	if ((mSignalStatus > 10) && (mNetStatus == 2))
	{
		DEBUG_PRINT_LN(F("Anti Jamming GSM with Service"));
		setFonaStatus(mNetStatus);//antijamming para gsm: Me preocupa que entre muy rápido a esta rutina y nunca se registre en la red por causa del reset constante
		//DEBUG_RAM("AntiJam GSM", Ram());
	}

	else if ((mSignalStatus > 5) && ((mNetStatus == 1) || ((mNetStatus == 3))))
	{
		checkSms();
//		DEBUG_RAM("SearchJSONline", freeRam());
		//Load de la última posición del logger que se subió correctamente
		storage->initFilePos = storage->loadJsonLine();
//		DEBUG_RAM("LoadJSONline");
		if (storage->initFilePos == -1)
		{
			DEBUG_PRINTT(F("Error leyendo pos_i"));
		}
		else
		{
			uploadData();
		}
	}
}



void Comms::setGsmCount(uint8_t count){
	gsmCount = count;
}

uint8_t Comms::getGsmCount(){
	return gsmCount;
}

void Comms::resetGsmCount(){
	gsmCount = tryGsm;
}
