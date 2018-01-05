/*
 filename: DebugRAM.h
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 Memory tracking & debugging macros.
 */

#ifndef DEBUG_RAM_H
#define DEBUG_RAM_H

#include <Arduino.h>
#include <DebugUtils.h>
#include <Persistence.h>

#ifdef DEBUG

extern "C" char *sbrk(int i); //Para RAM
uint32_t cycleCount = 0;
uint32_t generalCount = 0;
String data;

Persistence *storageDbg;

inline void setupRamDebug(Persistence persistence)
{
	storageDbg = &persistence;
}

//Rutina para calcular el espacio de memoria Ram disponible
inline int freeRam()
{
	char stackDummy = 0;
	return &stackDummy - sbrk(0);
}
#define DEBUG_RAM_SETUP(storage)\
	setupRamDebug(storage);
#define DEBUG_RAM_RESET_CYCLE()\
	cycleCount = 0;
#define DEBUG_RAM(donde)\
	data = String(cycleCount) + " " + String(donde) + ": " + String(freeRam()); \
	DEBUG_PRINTT(cycleCount); \
	DEBUG_PRINTT(" " + String(donde) + ": "); \
	DEBUG_PRINT_LN(freeRam()); \
	storageDbg->writeFile("ramtrack.txt", data); \
	cycleCount++;
#else
#define DEBUG_RAM(donde)
#endif

#endif
