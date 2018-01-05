/*
 filename: Watchdog.cpp
 author: Sergio Vargas
 date last modified: 05/01/2018
 platform: ARM M0 (SAMD21)
 SAMD21 watchdog timer library source file.
 */

#include "../Watchdog_SAMD21/Watchdog.h"

Watchdog::Watchdog(){
}

Watchdog::Watchdog(uint8_t timeout){
	WDT->CTRL.reg = 0; // Se desactiva WDT
	WDTsync(); // Sincronizar para no perder configuración

	GCLK->GENDIV.reg = 0x1F02; //Establece la división del generador de relojes (GCLKGEN2/2^5)
	GCLKsync(); // Sincronizar para no perder configuración

	GCLK->GENCTRL.reg = 0x110302; //Relaciona OSCULP32K con GCLKGEN2
	GCLKsync(); // Sincronizar para no perder configuración

	GCLK->CLKCTRL.reg = 0x4203; //Relaciona GCLKGEN2 con GCLK_WDT
	GCLKsync(); // Sincronizar para no perder configuración

	WDT->CONFIG.reg = min(timeout, wdtTimeout); // Tabla 17-5 Timeout Period

	WDT->CTRL.reg = WDT_CTRL_ENABLE; //Activar WDT
	WDTsync(); // Sincronizar para no perder configuración
}

//Sincronización del WDT
void Watchdog::WDTsync()
{
	while (WDT->STATUS.bit.SYNCBUSY == 1);
}

//Sincronización del GCLK
void Watchdog::GCLKsync()
{
	while (GCLK->STATUS.bit.SYNCBUSY == 1);
}

// reset del WDT (DEBE ser con 0xA5)
void Watchdog::resetWDT()
{
	WDT->CLEAR.reg = 0xA5;
	WDTsync();
}

// reset del sistema (DIFERENTE de 0xA5)
void Watchdog::systemReset()
{
	WDT->CLEAR.reg = 0x00;
	WDTsync();
}

// desactivar WDT
void Watchdog::disableWDT()
{
	WDT->CTRL.reg = 0;
	WDTsync();
}
