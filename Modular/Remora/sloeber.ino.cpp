#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-01-12 17:47:28

#include "Arduino.h"
#define DEBUG 1
#define SSID F("Javo")
#define PASS F("123456789")
#include <DebugUtils.h>
#include <DebugESP.h>
#include <DebugRAM.h>
#include <Persistence.h>
#include <Adafruit_FONA.h>
#include <Quadtree.h>
#include <Comms.h>
#include <Watchdog.h>
#include <wiring_private.h>

void SERCOM2_Handler() ;
void ledbugging(int frequency, boolean onoff) ;
void assertWrite(String str) ;
void uploadData() ;
bool pointInArea(const vector<Data>& pTestPoints, float lat, float lon) ;
void loadMap() ;
void setup() ;
void loop() ;

#include "Remora.ino"


#endif
