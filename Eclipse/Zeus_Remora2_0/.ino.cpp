#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-04-10 15:59:42

#include "Arduino.h"
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <avr/dtostrf.h>
#include "Adafruit_FONA.h"
#include "wiring_private.h"
#include <quadtree.h>
void SERCOM2_Handler() ;
void fona_setup() ;
void anti_jam(int typeofjamming);
void anti_jam_GPS();
int Net_Status();
void sd_setup() ;
void sercom_setup() ;
bool point_in_area(const vector<Data>& test_points, float lat, float lon) ;
boolean write_file(String file, String msg) ;
void Read_file (String file);
void Trunc_file(String file) ;
void load_config_params() ;
void load_net_config() ;
void load_srv_config() ;
void load_map();
unsigned long load_json_line();
void process_sms(int8_t sms_num) ;
void ledbugging(int frecuencia, boolean onoff);
String get_gps() ;
void assert_write(String str) ;
void Service_available();
void upload_data();
int fona_post(String url, String msg) ;
void setup() ;
void loop() ;

#include "Zeus_Remora2_0.ino"


#endif
