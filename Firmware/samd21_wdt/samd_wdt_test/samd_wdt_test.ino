#define Serial SERIAL_PORT_USBVIRTUAL

static void WDTsync() {
  while (WDT->STATUS.bit.SYNCBUSY == 1); //Sincronización del WDT
}

static void GCLKsync() {
  while (GCLK->STATUS.bit.SYNCBUSY == 1); //Sincronización del GCLK
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("WDT Test");

  setupWDT( 9 ); //Inicialización del WDT
  
  uint32_t t0 = millis(); 
  
  while(millis() - t0 <= 25000){ //Ciclo hasta 25s o hasta reset por WDT
    
    delay(100);
    if( millis()- t0 < 15000 ){
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
  
  GCLK->GENDIV.reg = 0x1F02; //Establece la división del generador de relojes (GCLKGEN2/2^5)
  GCLKsync(); // Sincronizar para no perder configuración
  
  GCLK->GENCTRL.reg = 0x110302; //Relaciona OSCULP32K con GCLKGEN2
  GCLKsync(); // Sincronizar para no perder configuración
  
  GCLK->CLKCTRL.reg = 0x4203; //Relaciona GCLKGEN2 con GCLK_WDT
  GCLKsync(); // Sincronizar para no perder configuración
  
  WDT->CONFIG.reg = min(period, 11); // Tabla 17-5 Timeout Period 

  WDT->CTRL.reg = WDT_CTRL_ENABLE; //Activar WDT
  WDTsync(); // Sincronizar para no perder configuración
}
