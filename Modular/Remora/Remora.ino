/*
 filename: Remora.ino
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 Remora main functions source file.
 */

#define DEBUG 1
//#define DEBUG_RAM 1
//#define WDEBUG 1
#define SSID F("Javo")  	//Nombre de la red a la que se desea conectar
#define PASS F("123456789") //Contraseña de la red

//=========================Includes==============================
#include <DebugUtils.h>
#include <DebugESP.h>
#include <DebugRAM.h>
#include <Persistence.h>
#include <Adafruit_FONA.h>
#include <Quadtree.h>
#include <Comms.h>
#include <Watchdog.h>
#include <wiring_private.h> //pinPeripheral

#define LED 25

String id = "XYZ001";

//Variables para el cálculo de distancia entre 2 puntos
float lastLat = 0;
float lastLon = 0;
Nmea gpsData;

//Variables auxiliares para cálculo de tiempo
long t1 = 0;
long t2 = 0;

//Definición de función de carga para pasar a comms
//void uploadData();

Persistence storage;
Quadtree remoraGis;
vector<Data> testPoints; //Estructura de datos para determinación de zona más cercana
Comms comms;
Watchdog wdt;

void SERCOM2_Handler()
{
	comms.Serial2.IrqHandler();
}

//=========================Para el LED==============================//
//Funcion led para debugging
void ledbugging(int frequency, boolean onoff)
{
	for (int i = 0; i <= 5; i++)
	{
		digitalWrite(LED, HIGH);
		delay(50 * frequency);
		digitalWrite(LED, LOW);
		delay(50 * frequency);
	}
	if (onoff)
	{
		digitalWrite(LED, HIGH);
	}
	else
	{
		digitalWrite(LED, LOW);
	}
}

//=========================Para REMORA==============================//
//Dado un mensaje con los datos del GPS (getNmea), se verifican los umbrales, escribe en el logger
void assertWrite(String str)
{
	if (str.indexOf("ERROR") == -1)
	{
		ledbugging(3, 1);
		//Cálculo del tiempo transcurrido desde la última escritura
		int elapsed = (int) ((t1 - t2) / 1000);
		DEBUG_PRINT_LN(F("--Analizando Umbrales--"));
		DEBUG_PRINTT(F("Tiempo transcurrido: "));
		DEBUG_PRINT_LN(elapsed);
		DEBUG_PRINTT(F("Distancia recorrida: "));
		DEBUG_PRINTT(F("Latitud: "));
		DEBUG_PRINTT(abs(gpsData.lat - lastLat));
		DEBUG_PRINTT(F(" Longitud: ")); DEBUG_ESP_LN(String(elapsed) + " " + String(abs(gpsData.lat - lastLat)) + " " + String(abs(gpsData.lon - lastLon)));
		if (( abs(gpsData.lat - lastLat) > storage.lineOfSight
				|| abs(gpsData.lon - lastLon) > storage.lineOfSight)
				|| elapsed > storage.timeTrigger)
		{
			DEBUG_RAM("RompeUmbral");
			lastLat = gpsData.lat;
			lastLon = gpsData.lon;
			t2 = millis();
			testPoints = remoraGis.nearest_points(gpsData.lat, gpsData.lon,
					storage.lineOfSight);
			DEBUG_RAM("VectorQT");
			//funcion motor
			//funcion gasolina
			float fuel = 1200.00;
			//{"ID":"XYZ001","fecha":20160914234855,"lat":-90.937282,"lon":-180.059265,"vel":999.99,"alt":10018.900,"Head":279.10,"RunStatus":1,"Fix":1,"FixMode":1,"HDOP":10.2,"PDOP":10.8,"VDOP":10.9,"GPSView":99,"GNSS_used":99,"HPA":9999.9,"VPA":9999.9,"fuel":0000.00,"Motor":1,"Quadtree":1
			str = str + ",\"fuel\":" + String(fuel) + ",\"QuadTree\":"
					+ String(pointInArea(testPoints, gpsData.lat, gpsData.lon));
			//str = str + ",\"fuel\":" + String(fuel) + ",\"Motor\":" + String(Motor) + ",\"QuadTree\":" + String(point_in_area(test_points, curr_lat, currentLon));
			storage.writeFile("log.txt", str);
			DEBUG_RAM("SaveLog");
			comms.uploadIfServiceAvailable();
//			generalCount++;			//Borrar después de conteo
		}
	}
	else
	{
		ledbugging(3, 0);
	}
}

//Logica para enviar línea por línea a la nube... improvement: enviar paquetes
void uploadData()
{

	File logFile = SD.open("log.txt");
	if (!logFile)
	{
		DEBUG_PRINT_LN(F("Log.txt not found"));
	}
	else
	{
		long fileSize = logFile.size();
		DEBUG_PRINTT(F("Size: "));
		DEBUG_PRINT_LN(fileSize);
		DEBUG_PRINTT(F("pos_i: "));
		DEBUG_PRINT_LN(storage.initFilePos);

		if (fileSize == 0)
		{
			DEBUG_PRINT_LN(F("Log vacio"));
		}
		else
		{
			String line;
			DEBUG_RAM("Seakposi");
			logFile.seek(storage.initFilePos);
			DEBUG_RAM("foundposi");
			line = logFile.readStringUntil('\n');		////Posible leak de RAM
			DEBUG_RAM("readuntil");
			storage.finalFilePos = logFile.position();
			DEBUG_RAM("posf");
			logFile.close();
			line.trim();
			DEBUG_RAM("JSONLoaded");

			if ((comms.post(storage.postUrl, line)) == 200)
			{
				ledbugging(2, 1);
				DEBUG_PRINT_LN(F("Nueva posicion en cursor"));
				storage.initFilePos = storage.finalFilePos;
				storage.truncFile("jsonline.txt");
				storage.writeFile("jsonline.txt", String(storage.initFilePos));
				DEBUG_RAM("SaveJSONline");

				if (fileSize == storage.initFilePos)
				{
					DEBUG_PRINT_LN(F("Fin del archivo. Trunc log..."));
					storage.truncFile("jsonline.txt");
					storage.writeFile("jsonline.txt", "0");
					storage.truncFile("log.txt");
					DEBUG_RAM("Emptylog");
				}
				else
				{
					comms.resetGsmCount();//Se coloca éste umbral para evitar una recursión y para que el próximo ciclo del loop llame a upload inmediatamente
				}
			}
			else
			{
				ledbugging(2, 0);
				DEBUG_PRINT_LN(F("Error en la subida de datos"));
			}
		}

	}
}

//=========================Para el QUADTREE==============================//
//Consulta si el punto de prueba se encuentra en las áreas dadas
bool pointInArea(const vector<Data>& pTestPoints, float lat, float lon)
{
	bool inside = false;
	for (auto i : pTestPoints)
	{
		inside = i.point_in_area(lat, lon);
		DEBUG_PRINTT(F("Colision con area "));
		DEBUG_PRINTT(i.pos.x);
		DEBUG_PRINTT(F(", "));
		DEBUG_PRINTT(i.pos.y);
		DEBUG_PRINTT(F(": "));
		DEBUG_PRINT_LN(inside);
		if (inside)
			break;
	}
	return inside;
}

//Cargar quadtree (mapa) a memoria
void loadMap()
{

}

//===========================================================================================//
void setup()
{
	pinMode(LED, OUTPUT);
//	digitalWrite(LED, HIGH);
//	delay(1000);
//	digitalWrite(LED, LOW);
//	delay(1000);

	DEBUG_PRINT_SETUP()
	DEBUG_ESP_SETUP();

	DEBUG_PRINT_LN(F("ESP F"));

	remoraGis = Quadtree();
	storage = Persistence(100);
	Serial.println("Prueba comms");
	comms = Comms(storage, &uploadData);
	//wdt = Watchdog(11);
//	DEBUG_PRINT_PRETTY(F("Comms F"));

//	digitalWrite(LED, HIGH);
//	delay(1000);
//	digitalWrite(LED, LOW);
//	delay(1000);

//	DEBUG_RAM("Inicio");

	storage.readFile("log.txt");
	storage.readFile("nonupdat.txt");
	storage.readFile("updat.txt");
	storage.readFile("ramtrack.txt");
	storage.readFile("srv.txt");

	//Trunc_file("log.txt");
	//Trunc_file("nonupdat.txt");
	//Trunc_file("updat.txt");
	//Trunc_file("ramtrack.txt");
	//Trunc_file("srv.txt");

	//write_file("srv.txt","S");
	//write_file("srv.txt","H");
	//write_file("srv.txt","imaginexyz-genuinoday.herokuapp.com/gps/today");
	//Read_file("srv.txt");

	comms.checkSms();

	//Cargar parámetros configurables
	storage.loadDeviceConfig();
	storage.loadNetConfig();
	storage.loadServerConfig();
	//loadMap();

	DEBUG_PRINT_LN(F("======SETUP LISTO======")); DEBUG_ESP_LN(F("======SETUP LISTO======"));

	DEBUG_RAM("Setup");
	//La Sabana
	remoraGis.insert(-84.110250f, 9.933845f, 0x1E1);
	remoraGis.insert(-84.108877f, 9.938325f, 0x2DF);
	remoraGis.insert(-84.098514f, 9.937848f, 0x2D4);
	remoraGis.insert(-84.098266f, 9.935712f, 0x2D2);
	remoraGis.insert(-84.099593f, 9.935712f, 0x2D6);
	remoraGis.insert(-84.099605f, 9.932733f, 0x2D2);

	//Office
	remoraGis.insert(-84.061049f, 9.937767f, 0xFFF);

	DEBUG_RAM("Quadtree");
}

void loop()
{
	DEBUG_RAM("Loop");
	//wdt.resetWDT();

	t1 = millis();	//agregar t1 para tomar datos de tiempo y romper umbral

	//Obtener datos de ubicación
	String statusString = comms.getNmea(&gpsData);
	DEBUG_RAM("GetGPS");
	if (statusString != "ERROR")
	{
		assertWrite(statusString); //Determinar si se ha roto algún umbral
	}

	//Rutina para realizar un reset al FONA en caso de que tenga señal y no esté registrado en la red
	comms.setGsmCount(comms.getGsmCount() + 1);
	if (comms.getGsmCount() >= comms.tryGsm)
	{
		comms.setGsmCount(0);
		comms.uploadIfServiceAvailable(); //No solo anti jam para gsm sino que si hay señal pero no hay GPS sube los datos
	}
	else
	{
		for (int x = 0; x < storage.delayTime; x++)
		{
			delay(1000);
			//wdt.resetWDT();
		}
	}
	int cuenta = 0;
	while (cuenta <= storage.delayTime)
	{
		//wdt.resetWDT();
		DEBUG_PRINTT(F("."));
		delay(1000);
		cuenta++;
	}
	DEBUG_PRINT_LN(F(".")); DEBUG_RAM_RESET_CYCLE(); ////////////////////////////////////Comment after RAM tracking
}
