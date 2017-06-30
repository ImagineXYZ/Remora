#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

#define cardSelect 4
const int chipSelect = 4;

File logfile;

int valI1=0;
int valI2=0;
int valV=0;

int A_2OE=5;
int A4_2A=6;
int A2_2A=9;
int A2_1A=10;
int A4_1A=11;
int A_1OE=12;
float k1=0,k2=0;
int b1=0,b2=0;
long previousTime= 0;
long sdTime=0;
float Imax1=0;
float Imax2=0;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

//A2=OPEN, A4=OPEN
void Monitor1_setG10(){
	digitalWrite(A_1OE,HIGH);
	digitalWrite(A4_1A, LOW);
	digitalWrite(A2_1A,LOW);
	k1=2*10*(1/10.0)*3.3/4096.0;
	b1=25;
}

//A2=VEE, A4=VEE
void Monitor1_setG50(){
	digitalWrite(A_1OE,LOW);
	digitalWrite(A4_1A, LOW);
	digitalWrite(A2_1A,LOW);
	k1=2*10*(1/50.0)*3.3/4096.0;
	b1=45;
}

//A2=OPEN, A4=OPEN
void Monitor2_setG10(){
	digitalWrite(A_2OE,HIGH);
	digitalWrite(A4_2A, LOW);
	digitalWrite(A2_2A,LOW);
	k2=10*(1/10.0)*3.3/4096.0;
	b2=45;
}

//A2=VEE, A4=VEE
void Monitor2_setG50(){
	digitalWrite(A_2OE,LOW);
	digitalWrite(A4_2A, LOW);
	digitalWrite(A2_2A,LOW);
	k2=10*(1/50.0)*3.3/4096.0;
	b2=45;
}

void setup()
{
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	pinMode(13, OUTPUT);
	pinMode(A_2OE,OUTPUT);
	pinMode(A4_2A,OUTPUT);
	pinMode(A2_2A,OUTPUT);
	pinMode(A2_1A,OUTPUT);
	pinMode(A4_1A,OUTPUT);
	pinMode(A_1OE,OUTPUT);
	Monitor1_setG10();
	Monitor2_setG50();
	analogReadResolution(12);


	if (!SD.begin(cardSelect)) {
		Serial.println("Card init. failed!");
		error(2);
	}
	char filename[20];
	strcpy(filename, "EnergyMonitor00.TXT");
	for (uint8_t i = 0; i < 100; i++) {
		filename[13] = '0' + i/10;
		filename[14] = '0' + i%10;
		// create if does not exist, do not open existing, write, sync after write
		if (! SD.exists(filename)) {
			break;
		}
	}
	logfile = SD.open(filename, FILE_WRITE);
	if( ! logfile ) {
		Serial.print("Couldnt create ");
		Serial.println(filename);
		error(3);
		while(1){}
	}
	Serial.print("Writing to ");
	Serial.println(filename);

	Serial.println("Ready!");
}


void measure(){
	valI1 = analogRead(A0);
	valI2 = analogRead(A1);
	valV = analogRead(A2);
}


void measureRaw(){
	valI1 = analogRead(A0);
	valI2 = analogRead(A1);
	valV = analogRead(A2);
}

void SdOutput() {

  logfile.print(valI1);
  logfile.print(",");
  logfile.print(valI2);
  logfile.print(",");
  logfile.print(valV);
  logfile.print(",");
  logfile.println(sdTime);   // Print battery voltage
  logfile.flush();
}

void loop()
{
	long measureTime;
	previousTime=micros();
	measureRaw();
	measureTime=micros()-previousTime;
	SdOutput();
	sdTime=micros()-previousTime;
	Serial.print(valI1);
	Serial.print(",\t");
	Serial.print(valI2);
	Serial.print(",\t");
	Serial.print(valV,4);
	Serial.print(", Meas=");
	Serial.print(measureTime);
	Serial.print(", SDTime=");
	Serial.println(measureTime);
}
