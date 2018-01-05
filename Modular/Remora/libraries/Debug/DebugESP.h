/*
 filename: DebugESP.h
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 Debugging macros for wireless debugging using ESP8266.
 */

#ifndef DEBUG_ESP_H
#define DEBUG_ESP_H

#include <Arduino.h>
#include <ESP8266_XYZ.h>
#include <DebugUtils.h>
#include <wiring_private.h>

#if defined(WDEBUG) && defined(SSID) && defined(PASS)
Uart WirelessSerial = Uart(&sercom1, 13, 11, SERCOM_RX_PAD_1, UART_TX_PAD_0); //Sercom1 multiuso
void SERCOM1_Handler()
{	WirelessSerial.IrqHandler();}
ESP8266_XYZ espDev(&WirelessSerial); //Arg #1: puntero al stream, Arg #2: pin para reset
#define DEBUG_ESP_SETUP() \
	WirelessSerial.begin(115200); \
	pinPeripheral(13, PIO_SERCOM); \
	pinPeripheral(11, PIO_SERCOM); \
	while (!espDev.espTest()); \
	DEBUG_PRINT_LN("ESP Test"); \
	while (!espDev.softReset()); \
	DEBUG_PRINT_LN("ESP Reset"); \
	while (!espDev.connectAP(SSID, PASS)){delay(5000);} \
	DEBUG_PRINT_LN("ESP OK");
#define DEBUG_ESP_PRETTY(str)    \
   espDev.print(millis());     \
   espDev.print(": ");    \
   espDev.print(__PRETTY_FUNCTION__); \
   espDev.print(' ');      \
   espDev.print(__FILE__);     \
   espDev.print(':');      \
   espDev.print(__LINE__);     \
   espDev.print(' ');      \
   espDev.println(str);
#define DEBUG_ESP_LN(str)    \
   espDev.println(str);
#define DEBUG_ESP(str)    \
   espDev.print(str);
#else
#define DEBUG_ESP_SETUP()
#define DEBUG_ESP_PRETTY(str)
#define DEBUG_ESP_LN(str)
#define DEBUG_ESP(str)
#endif

#endif
