#define Serial SERIAL_PORT_USBVIRTUAL

static void WDTsync() {
  while (WDT->STATUS.bit.SYNCBUSY == 1); //Sincronización del WDT
}

void setup() {
  Serial.begin(115200);
  delay(15000); //Tiempo para inicializar hardware

  Serial.println("WDT Test");

  setupWDT( 11 ); //Inicialización del WDT
  
  uint32_t t0 = millis(); 
  
  while(millis() - t0 <= 25000){ //Ciclo hasta 25s o hasta reset por WDT
    
    delay(10);
    if( millis()- t0 <15000 ){
      resetWDT();
    }

    Serial.println( millis()- t0 ); //imprimir el tiempo en ms

  }
  Serial.println("No hay reset del sistema");
  disableWDT();
  
  while(true);
}

void loop() {
}

void resetWDT() {
  WDT->CLEAR.reg = 0xA5; // reset del WDT (DEBE ser con 0xA5)
  WDTsync(); 
}

void systemReset() {
  WDT->CLEAR.reg = 0x00; // reset del sistema (DIFERENTE de 0xA5)
  WDTsync(); 
}

void disableWDT(){
  WDT->CTRL.reg = 0; // desacttivar WDT
  WDTsync();
}

void setupWDT(uint8_t period) {

  WDT->CTRL.reg = 0; // Se desactiva WDT
  WDTsync(); // Sincronizar para no perder configuración

  WDT->CONFIG.reg = min(period, 11); // Tabla 17-5 Timeout Period 

  WDT->CTRL.reg = WDT_CTRL_ENABLE; //Activar WDT
  WDTsync(); // Sincronizar para no perder configuración
}
