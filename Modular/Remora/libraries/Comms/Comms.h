/*
 filename: Comms.h
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 Remora communications module header file.
 */

#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
#include <Adafruit_FONA.h>
#include <Persistence.h>
#include <wiring_private.h> //pinPeripheral
#include <DebugUtils.h>
#include <DebugESP.h>
#include <DebugRAM.h>

#define FONA_RI 8
#define FONA_KEY 9
#define FONA_RST 18 //(A4)
#define FONA_NS 17 //(A3)

typedef struct Nmea
{
	float lat, lon, speedKph, heading, altitude;
	double date;bool run, fix;
	float hdop, pdop, vdop, hpa, vpa;
	uint8_t fixMode, gpsView, gnsUsed;
} NmeaT;

typedef void (*upload_t)();

class Comms
{
public:
	Comms();
	Comms(Persistence &persistence, upload_t function);
	void setFonaStatus(int);
	void checkSms();
	int post(String, String);
	String getNmea(Nmea *);
	void rebootGps();
	void uploadIfServiceAvailable();
	upload_t uploadData;
	int netStatus();
	void setGsmCount(uint8_t);
	void resetGsmCount();
	uint8_t getGsmCount();
	Uart Serial2 = Uart(&sercom2, 3, 4, SERCOM_RX_PAD_1, UART_TX_PAD_0);
	//NOTA a cambiar: Se utiliza el sercom2_alt debería ser el sercom0 mismos pines
	static const uint8_t tryGsm = 5; //Cantidad de intentos antes de reset general del FONA
	static const uint8_t tryGps = 15; //Cantidad de intentos antes del on/off en GPS del FONA
	static const uint8_t tryGprs = 5; //Cantidad de intentos antes del on/off en GPRS

private:
	HardwareSerial *fonaSerial;
	Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
	Persistence storage;
	Nmea nmeaData;
	String deviceId;
	uint8_t gsmCount = 0;
	uint8_t noGpsCount = 0;
};

#endif
