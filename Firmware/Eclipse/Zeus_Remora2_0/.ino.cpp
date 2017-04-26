#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-04-25 09:10:05

#include "Arduino.h"
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <avr/dtostrf.h>
#include "Adafruit_FONA.h"
#include <quadtree.h>
#include <ESP8266_XYZ.h>
#include "wiring_private.h"
void SERCOM1_Handler();
void SERCOM2_Handler();
void fona_setup() ;
void sd_setup() ;
void sercom_setup() ;
void esp_setup();
boolean write_file(String file, String msg) ;
void Read_file (String file);
void Trunc_file(String file) ;
void load_config_params() ;
void load_net_config() ;
void load_srv_config() ;
unsigned long load_json_line();
void ledbugging(int frecuencia, boolean onoff);
void anti_jam(int typeofjamming);
void anti_jam_GPS();
int Net_Status();
void process_sms(int8_t sms_num) ;
String get_gps() ;
int fona_post(String url, String msg) ;
void assert_write(String str) ;
void Service_available();
void upload_data();
bool point_in_area(const vector<Data>& test_points, float lat, float lon) ;
void load_map();
void setup() ;
void loop() ;

#include "Zeus_Remora2_0.ino"


#endif
