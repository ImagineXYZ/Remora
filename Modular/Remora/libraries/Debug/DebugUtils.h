/*
 filename: DebugUtils.h
 author: Sergio Vargas
 date last modified: 21/12/2017
 platform: ARM M0 (SAMD21)
 Fabio Varesano's simple debugging utilities modified for custom needs.


 Copyright (C) 2011 Fabio Varesano <fabio at varesano dot net>

 Ideas taken from:
 http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1271517197

 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <Arduino.h>

#ifdef DEBUG
//Serial port config for ARM M0 (SAMD21)
#define Serial SERIAL_PORT_USBVIRTUAL //Puerto USB SerialUSB
#define DEBUG_PRINT_SETUP() \
	Serial.begin(9600); \
	while(!Serial); \
	Serial.println(F("======DEBUG======"));
#define DEBUG_PRINT_PRETTY(str)    \
   Serial.print(millis());     \
   Serial.print(": ");    \
   Serial.print(__PRETTY_FUNCTION__); \
   Serial.print(' ');      \
   Serial.print(__FILE__);     \
   Serial.print(':');      \
   Serial.print(__LINE__);     \
   Serial.print(' ');      \
   Serial.println(str);
#define DEBUG_PRINT_LN(str)    \
   Serial.println(str);
#define DEBUG_PRINTT(str)    \
   Serial.print(str);
#else
#define DEBUG_PRINT_SETUP
#define DEBUG_PRINT_PRETTY(str)
#define DEBUG_PRINT_LN(str)
#define DEBUG_PRINTT(str)
#endif

#endif
