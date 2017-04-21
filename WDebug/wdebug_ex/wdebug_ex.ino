//Bibliotecas que se deben incluir
#include <ESP8266_XYZ.h>
#include "wiring_private.h" //pinPeripheral

//#define SSID F("WDebug")  //Nombre de la red a la que se desea conectar
//#define PASS F("espdebug")   //Contraseña de la red

//#define SSID F("CarArt")  //Nombre de la red a la que se desea conectar
//#define PASS F("puravida2016")   //Contraseña de la red

#define SSID F("Javo")  //Nombre de la red a la que se desea conectar
#define PASS F("123456789")   //Contraseña de la red

#define Serial SERIAL_PORT_USBVIRTUAL

Uart WSerial (&sercom1, 13, 11, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void SERCOM1_Handler()
{
  WSerial.IrqHandler();
}

ESP8266_XYZ esp(&WSerial);    //Arg #1: puntero al stream, Arg #2: pin para reset

int n = 20000;
int m = 20000;

void setup() {
  
  
  Serial.begin(9600);             //Inicializacion del Monitor Serial a 9600
  while(!Serial);    
  Serial.println("Init");         //Mensaje de inicialización
  WSerial.begin(9600);           //Inicializacion de SoftwareSerial a 9600
  Serial.println("Init WSerial");         //Mensaje de inicialización

  pinPeripheral(13, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
  
  //No se continúa hasta asegurar el buen funcionamiento del dispositivo
  while (!esp.espTest());
  Serial.println("ESP Test");  
  while (!esp.softReset());
  Serial.println("ESP Reset");
  esp.connectAP(SSID, PASS);
  Serial.println("ESP AP");
  
  Serial.println("Setup OK");     //Si se muestra existe conexión
  
}

void loop() {

  esp.println(String(n));
  Serial.println(String(n));

  //Se incrementa contador de ejemplo 
  n++;
  //Se decrementa el contador de ejemplo
  m--;
  
  delay(1000);         //Suspende operaciones por 1 segundos
}
