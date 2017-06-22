#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

//Definiciones de la SD
#define cardSelect 4

//Variables para Manejo de Archivos
File logfile;
char filename[15];

//Variables para captura de datos
int valI1=0;
int valI2=0;
int valV1=0;
long timestamp= 0; //función micros

//Escogencia de ganancias
int GI1=10;
int GI2=50;

//Variables para conversión de datos
float GV1=4.22*3.3/4096.0;
float k1=0,k2=0; //Se modifican al configurar ganacias (Setup)
float b1=0,b2=0;

//Pines de control de los monitores de corriente
int A_2OE=5;
int A4_2A=6;
int A2_2A=9;
int A2_1A=10;
int A4_1A=11;
int A_1OE=12;

//Pin de Botón
int pinBoton=13;

//Variables de control de flujo del algoritmo
int medir=1;
int almacenar=1;
int imprimir=0;
//const String RawInfoSD="G1="+String(GI1)+", G2="+String(GI2)+", GV1="+String(GV1);
int error=0;

// blink out an error code
void Error(uint8_t errno) {
	uint8_t i;
	for (i=0; i<errno; i++) {
		//digitalWrite(13, HIGH);
		delay(100);
		//digitalWrite(13, LOW);
		delay(100);
	}
	for (i=errno; i<10; i++) {
		delay(200);
	}
}

//A2=OPEN, A4=OPEN
void Monitor1_setG10(){
	digitalWrite(A_1OE,HIGH);
	digitalWrite(A4_1A, LOW);
	digitalWrite(A2_1A,LOW);
	k1=2*10*(1/10.0)*3.3/4096.0;
	//b1=25;
	//Modificado para enviar
	//y con corrección de los datos graficamente
	b1=-0.055283203;
}

//A2=VEE, A4=VEE
void Monitor1_setG50(){
	digitalWrite(A_1OE,LOW);
	digitalWrite(A4_1A, LOW);
	digitalWrite(A2_1A,LOW);
	k1=2*10*(1/50.0)*3.3/4096.0;
	//b1=45;
	//Modificado para enviar
	b1=-45*k2;

}

//A2=OPEN, A4=OPEN
void Monitor2_setG10(){
	digitalWrite(A_2OE,HIGH);
	digitalWrite(A4_2A, LOW);
	digitalWrite(A2_2A,LOW);
	k2=10*(1/10.0)*3.3/4096.0;
	//Modificado para enviar
	//b2=45;
	b2=-45*k2;
}

//A2=VEE, A4=VEE
void Monitor2_setG50(){
	digitalWrite(A_2OE,LOW);
	digitalWrite(A4_2A, LOW);
	digitalWrite(A2_2A,LOW);
	k2=10*(1/50.0)*3.3/4096.0;
	//b2=45;
	b2=-45*k2;
}

int IniciarArchivo(){
	//Asignar nombre base
	strcpy(filename, "MEAS0000.TXT");
	//Generar consecutivo
	for (int i = 0; i < 10000; i++) {
		filename[4] = '0' + (i/1000)%10;
		filename[5] = '0' + (i/100)%10;
		filename[6] = '0' + (i/10)%10;
		filename[7] = '0' + i%10;
		// create if does not exist, do not open existing, write, sync after write
		if (! SD.exists(filename)) {
			break;
		}
	}
	//Crear Archivo
	logfile = SD.open(filename, FILE_WRITE);
	if( ! logfile ) {
		Serial.print("Couldnt create ");
		Serial.println(filename);
		Error(3);
		error=3;
		return -1;
	}else{
		logfile.print("GI1=");
		logfile.print(GI1);
		logfile.print("[A]");
		logfile.print(",GI2=");
		logfile.print(GI2);
		logfile.print("[A]");
		logfile.print(",GV1=");
		logfile.print(GV1,5);
		logfile.print("[V]");
		logfile.print(",Timestamp[us]");
		logfile.print("|| kI1=");
		logfile.print(k1,5);
		logfile.print(", bI1=");
		logfile.print(b1,5);
		logfile.print(",kI2=");
		logfile.print(k2,5);
		logfile.print(", bI2=");
		logfile.print(b2,5);
		logfile.print(", kV1=");
		logfile.println(GV1,5);
		logfile.close();
		logfile.flush();
	}

	return 0;
}

int IniciarArchivoRaw(){
	//Asignar nombre base
	strcpy(filename, "MRAW0000.TXT");
	//Generar consecutivo
	for (int i = 0; i < 10000; i++) {
		filename[4] = '0' + (i/1000)%10;
		filename[5] = '0' + (i/100)%10;
		filename[6] = '0' + (i/10)%10;
		filename[7] = '0' + i%10;
		// create if does not exist, do not open existing, write, sync after write
		if (! SD.exists(filename)) {
			break;
		}
	}
	//Crear Archivo
	logfile = SD.open(filename, FILE_WRITE);
	if( ! logfile ) {
		Serial.print("Couldnt create ");
		Serial.println(filename);
		Error(3);
		error=3;
		return -1;
	}else{
		//Encabezado de archivo
		//Orden de columnas
		logfile.print("Col1:IMonitor1, Col2:IMonitor2, Col3: VL, Col4: Timestamp[us], Col5:error_code");
		//Datos para conversión
		logfile.print("|| Configuración de Ganancias ");
		logfile.print("GMonitorI1=");
		logfile.print(GI1);
		logfile.print("[V/V]");
		logfile.print(",GMonitorI2=");
		logfile.print(GI2);
		logfile.print("[V/V]");
		logfile.print(",GV1=");
		logfile.print(GV1,5);
		logfile.print("[V/V]");
		logfile.print("|| (f(x)=x*k+b);kI1=");
		logfile.print(k1,7);
		logfile.print("[A], bI1=");
		logfile.print(b1,7);
		logfile.print("[A],kI2=");
		logfile.print(k2,7);
		logfile.print("[A], bI2=");
		logfile.print(b2,7);
		logfile.print("[A], kV1=");
		logfile.println(GV1,7);
		logfile.print("[V]");
		logfile.close();
		logfile.flush();
	}
	return 0;
}

void medirRaw(){
	valI1=analogRead(A0);
	valI2=analogRead(A1);
	valV1=analogRead(A2);
	timestamp=micros();
}

void EscribirSDDataRaw() {
	logfile.print(valI1);
	logfile.print(",");
	logfile.print(valI2);
	logfile.print(",");
	logfile.print(valV1);
	logfile.print(",");
	logfile.print(timestamp);
	logfile.print(",");
	logfile.println(error);   // Print battery voltage
	logfile.close();
	logfile.flush();
}

void EscribirSDDataCalc() {
	logfile.print((valI1)*k1+b1,4);
	logfile.print(",");
	logfile.print((valI2)*k2+b2,4);
	logfile.print(",");
	logfile.print(valV1*GV1,4);
	logfile.print(",");
	logfile.print(timestamp);
	logfile.print(",");
	logfile.println(error);   // Print battery voltage
	logfile.close();
	logfile.flush();
}

void EnviarDatosSerial(){
	Serial.print((valI1)*k1+b1,4);
	Serial.print(",\t");
	Serial.print((valI2)*k2+b2,4);
	Serial.print(",\t");
	Serial.print(valV1*GV1,4);
	Serial.print(",\t Timestamp=");
	Serial.print(timestamp/1000.0,3);
	Serial.println("ms");
}


void ConfigurarGanancias(){
	switch(GI1){
	case 10:
		Monitor1_setG10();
		break;
	case 50:
		Monitor1_setG50();
		break;
	default:
		GI1=10;
		Monitor1_setG10();
	}
	switch(GI2){
	case 10:
		Monitor2_setG10();
		break;
	case 50:
		Monitor2_setG50();
		break;
	default:
		GI2=10;
		Monitor2_setG10();
	}
}

void setup()
{
	//Configuración de Pins


	pinMode(pinBoton, OUTPUT);
	digitalWrite(pinBoton,HIGH);
	pinMode(pinBoton, INPUT);
	pinMode(A_2OE,OUTPUT);
	pinMode(A4_2A,OUTPUT);
	pinMode(A2_2A,OUTPUT);
	pinMode(A2_1A,OUTPUT);
	pinMode(A4_1A,OUTPUT);
	pinMode(A_1OE,OUTPUT);

	Serial.begin(115200);

	ConfigurarGanancias();

	//Monitor1_setG10();
	//Monitor2_setG50();

	//Aumentar la resolución del ADC
	analogReadResolution(12);

	if (!SD.begin(cardSelect)) {
		Serial.println("Card init. failed!");
		Error(2);
		error=2;
		imprimir=1;
	}else{
		IniciarArchivoRaw();
	}


}


void loop()
{
	if(medir==1){
		medirRaw();
		if(almacenar==1){
			logfile = SD.open(filename, FILE_WRITE);
			if(logfile){
				//EscribirSDDataCalc();
				EscribirSDDataRaw();
			}else{
				error=4;
				imprimir=1;
			}
		}
		if(imprimir==1){
			EnviarDatosSerial();
		}
	}

	if(digitalRead(pinBoton)==0){
		delay(100);
		if(digitalRead(pinBoton)==0){
			Serial.println("Boton");
			IniciarArchivoRaw();
			while(digitalRead(pinBoton)==0){

			}
		}
	}
}
