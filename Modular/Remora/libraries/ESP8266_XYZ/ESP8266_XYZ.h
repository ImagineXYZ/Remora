/*
filename: ESP8266_XYZ.h
author: Sergio Vargas
date last modified: 05/01/2018
platform: ARM M0 (SAMD21)
Custom ESP8266 library (legacy) header file.
*/

#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>

class ESP8266_XYZ 
{
	public:
		ESP8266_XYZ(Stream *s = &Serial, int rst = -1);
		bool espTest();
		bool find_serial(int ser_timeout, String str);
		bool serial_line(int i, String str);
		bool connectAP(const __FlashStringHelper *ssid, const __FlashStringHelper *pass);
		bool softReset();
		bool hardReset();
		int readResponse(String* response);
		int httpPost(String server, String path, int port, String *response);
		int httpGet(String server, String path, int port, String *response);
		bool getJsonAttribute(String Input, String Attribute, String *value);
		void addToJson(String id, String value);
		void addToJson(String id, int value);
		void addToJson(String id, float value);
		void setTimeout(uint32_t timeout);
		void print(String msg);
		void println(String msg);
		
	private:
		void readSerialContent(int ser_timeout);
		bool connectServer(String server, int port);
		void addToJsonAux(String id, String value);
		int rst;
		String json = "{";
		String serial_content = "";
		Stream *stream;
		unsigned int global_timeout = 5000;

};

#endif
