/*
filename: Persistence.cpp
author: Sergio Vargas
date last modified: 05/01/2018
platform: ARM M0 (SAMD21)
Remora storage module source file.
*/

#include <Persistence.h>

Persistence::Persistence(){

}

Persistence::Persistence(uint8_t initTime){
	DEBUG_PRINT_LN(F("SD 0"));
	//Se establece comunicación con la SD
	while (!SD.begin(SD_CS))
	{
		delay(initTime);
	}
	DEBUG_PRINT_LN(F("SD lista"));
}


boolean Persistence::writeFile(String file, String msg)
{
	File fileOut;
	fileOut = SD.open(file, FILE_WRITE);
	//Escribir solo si el archivo se abrió adecuadamente
	if (fileOut)
	{
		DEBUG_PRINTT(F("Escribiendo en "));
		DEBUG_PRINTT(file);
		DEBUG_PRINTT(F(": "));
		DEBUG_PRINT_LN(msg);DEBUG_ESP_LN(msg);
		fileOut.println(msg);
		fileOut.close(); //Se cierra el archivo
		return true;
	}
	else
	{
		DEBUG_PRINT_LN(F("Error SD"));DEBUG_ESP_LN(F("Error SD"));
		return false;
	}
}

//Lee un archivo de SD e imprime en pantalla
void Persistence::readFile(String file)
{
	File fileOut;
	fileOut = SD.open(file);
	if (fileOut)
	{
		DEBUG_PRINTT(F("Reading "));
		DEBUG_PRINT_LN(file);
		while (fileOut.available())
		{
			DEBUG_PRINTT(fileOut.read());
		}
		fileOut.close();
	}
	else
	{
		DEBUG_PRINTT(F("Error opening "));
		DEBUG_PRINT_LN(file);DEBUG_ESP_LN(F("Error opening SD"));
	}
}

boolean Persistence::removeFile(String fileName){
	return SD.remove(fileName);
}

//Trunca un archivo SD (0 bytes)
void Persistence::truncFile(String file)
{
	File fileOut;
	fileOut = SD.open(file, O_WRITE | O_TRUNC);
	fileOut.close();
}


//Cargar las variables de configuración a memoria
void Persistence::loadDeviceConfig()
{
	File configFile = SD.open("config.txt");
	configFile.readStringUntil('\n');
	String line;
	if (configFile)
	{
		for (int i = 0; i < 4; i++)
		{
			line = configFile.readStringUntil('\n');
			DEBUG_PRINTT(F("Config Linea: "));
			line.trim();
			DEBUG_PRINT_LN(line);DEBUG_ESP_LN(line);
			switch (i)
			{
			case 0:
				lineOfSight = line.toFloat();
				break;
			case 1:
				timeTrigger = (uint16_t) (line.toInt());
				break;
			case 2:
				delayTime = (uint8_t) (line.toInt());
				break;
			case 3:
				emergencyMode = (uint8_t) (line.toInt());
				break;
			}
		}
		configFile.close();
	}
}

//Cargar las variables de red a memoria
//Agregar función para reasignar los parámetros de red celular (ON/OFF del FONA???)
void Persistence::loadNetConfig()
{
	File netFile = SD.open("net.txt");
	netFile.readStringUntil('\n');
	String line;
	if (netFile)
	{
		for (int i = 0; i < 3; i++)
		{
			line = netFile.readStringUntil('\n');
			DEBUG_PRINTT(F("Net Linea: "));
			line.trim();
			DEBUG_PRINT_LN(line);DEBUG_ESP_LN(line);
			switch (i)
			{
			case 0:
				netApn = line;
				break;
			case 1:
				netUser = line;
				break;
			case 2:
				netPassword = line;
				break;
			}
		}
		netFile.close();
	}
}

//Cargar las variables de servidor a memoria
void Persistence::loadServerConfig()
{
	File serverFile = SD.open("srv.txt");
	serverFile.readStringUntil('\n');
	String line;
	if (serverFile)
	{
		for (int i = 0; i < 2; i++)
		{
			line = serverFile.readStringUntil('\n');
			DEBUG_PRINTT(F("Server_Linea: "));
			line.trim();
			DEBUG_PRINT_LN(line);DEBUG_ESP_LN(line);
			switch (i)
			{
			case 0:
				getUrl = line;
				break;
			case 1:
				postUrl = line;
				break;
			}
		}
		serverFile.close();
	}
}

//Retorna la última línea que se subió correctamente al server, lo lee de un archivo en la SD
unsigned long Persistence::loadJsonLine()
{
	File lineFile = SD.open("jsonline.txt");
	String line = lineFile.readStringUntil('\n');
	if (lineFile)
	{
		line.trim();
		signed long logline = line.toInt();
		lineFile.close();
		return logline;
	}
	else
	{
		DEBUG_PRINT_LN(F("Error opening jsonline.txt"));DEBUG_ESP_LN(F("Error opening jsonline.txt"));
		return -1;
	}
}
