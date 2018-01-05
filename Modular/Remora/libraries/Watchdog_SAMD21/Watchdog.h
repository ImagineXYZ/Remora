/*
 filename: Watchdog.h
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 SAMD21 watchdog timer library header file.
 */

#ifndef WDT_SAMD21_H
#define WDT_SAMD21_H

#include <Arduino.h>

class Watchdog
{
public:
	Watchdog();
	Watchdog(uint8_t);
	void resetWDT();
	void systemReset();
	void disableWDT();
private:
	void WDTsync();
	void GCLKsync();
	static const uint8_t wdtTimeout = 11;
};

#endif
