/*
filename: Persistence.h
author: Sergio Vargas
date last modified: 05/01/2018
platform: ARM M0 (SAMD21)
Remora storage module header file.
*/

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <DebugUtils.h>
#include <DebugESP.h>

#define SD_CS 15  //(A1)

class Persistence {
public:
	Persistence();
	Persistence(uint8_t);
	boolean writeFile(String, String);
	void readFile(String);
	boolean removeFile(String);
	void truncFile(String);
	void loadDeviceConfig();
	void loadNetConfig();
	void loadServerConfig();
	unsigned long loadJsonLine();

	//Variables para parámetros umbrales
	float lineOfSight = 0.005f;
	uint16_t timeTrigger = 90; //
	uint8_t delayTime = 15;
	bool emergencyMode = false; //Por ahora no se utiliza

	//Variables para parámetros de red celular
	String netApn = "";
	String netUser = "";
	String netPassword = "";

	//Variables para parámetros de servidor
	String getUrl = "";
	String postUrl = "";

	//Auxiliares para el manejo de memoria durante la subida de datos
	long initFilePos = 0; //posición inicial del ultimo archivo log.txt que se subió con respuesta del servidor 200
	long finalFilePos = 0; //posición final del ultimo archivo log.txt que se subió con respuesta del servidor 200

	const char* deviceConfigFile = "config.txt";
	const char* netConfigFile = "net.txt";
	const char* serverConfigFile = "srv.txt";

private:
//	SD sdStorage;

};

#endif
