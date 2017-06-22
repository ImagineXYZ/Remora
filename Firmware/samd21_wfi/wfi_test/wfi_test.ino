
#define Serial SERIAL_PORT_USBVIRTUAL

static void GCLKsync() {
  while (GCLK->STATUS.bit.SYNCBUSY == 1); //Sincronización del GCLK
}

static void RTCsync() {
  while (RTC->MODE0.STATUS.bit.SYNCBUSY == 1); //Sincronización del RTC
}

uint32_t t0;
uint32_t t1;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("RTC Test");

  setupRTC();
  delay(100);
  wfi();
  
}
void loop() {
  
  
}


void wfi(){
  //Serial.println("------------CFG0------------");
  PM->SLEEP.reg = 0x2;  //Idle Mode
  
  //Serial.println("------------CFG1------------");

  RTC->MODE0.CTRL.reg = 0x0;  //Desactivar RTC
  RTCsync();
  RTC->MODE0.CTRL.reg = 0x1;  //Limpiar todos los registros de RTC
  RTCsync();
  //Serial.println("------------CFG2------------");

  RTC->MODE0.CTRL.reg = 0x0A00;   //Prescaler RTC_CLK/1024
  RTCsync();
  RTC->MODE0.INTENSET.reg = 0x1;  //Activar interrupciones de RTC
  //Serial.println("------------CFG3------------");
  
  RTC->MODE0.COMP[0].reg = 5000;    //Cantidad de ciclos para el contador
  RTC->MODE0.INTFLAG.reg = 0xFF;    //Limpiar interrupciones pendientes
  //Serial.println("------------CFG4------------");

  NVIC_SetPriority (RTC_IRQn, 0x3);   //Establecer prioridad 3 para las interrupciones de RTC
  NVIC_EnableIRQ(RTC_IRQn);           //Activa interrupciones de RTC
  //Serial.println("------------CFG5------------");

  RTC->MODE0.CTRL.reg |= 0x2;   //Activa RTC
  RTCsync();

  SCB->SCR |= 0x2;    //Deep Sleep
  __WFI();  //Instrucción WFI (Wait For Interrupt)
}

void RTC_Handler(void)
{
  Serial.println("------------INT------------");
  RTC->MODE0.INTFLAG.reg = 0xFF;  //Limpiar interrupciones pendientes
  RTC->MODE0.CTRL.reg = 0x0;      //Desactivar RTC
  RTCsync();
  wfi();
}

void setupRTC(){
  GCLK->GENDIV.reg = 0x1F03; 
  GCLK->GENCTRL.reg = 0x010403;
  GCLK->CLKCTRL.reg = 0x4304;
  GCLKsync();
}
