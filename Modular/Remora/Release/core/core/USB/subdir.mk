################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/CDC.cpp \
/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/PluggableUSB.cpp \
/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/USBCore.cpp 

C_SRCS += \
/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/samd21_host.c 

C_DEPS += \
./core/core/USB/samd21_host.c.d 

AR_OBJ += \
./core/core/USB/CDC.cpp.o \
./core/core/USB/PluggableUSB.cpp.o \
./core/core/USB/USBCore.cpp.o \
./core/core/USB/samd21_host.c.o 

CPP_DEPS += \
./core/core/USB/CDC.cpp.d \
./core/core/USB/PluggableUSB.cpp.d \
./core/core/USB/USBCore.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
core/core/USB/CDC.cpp.o: /home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/CDC.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Persistence" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/ESP8266_XYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/JsonXYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Debug" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes/platform" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Comms" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Quadtree" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Watchdog_SAMD21" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1/src" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core/core/USB/PluggableUSB.cpp.o: /home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/PluggableUSB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Persistence" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/ESP8266_XYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/JsonXYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Debug" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes/platform" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Comms" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Quadtree" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Watchdog_SAMD21" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1/src" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core/core/USB/USBCore.cpp.o: /home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/USBCore.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Persistence" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/ESP8266_XYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/JsonXYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Debug" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes/platform" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Comms" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Quadtree" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Watchdog_SAMD21" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1/src" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core/core/USB/samd21_host.c.o: /home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino/USB/samd21_host.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-gcc" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Persistence" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/ESP8266_XYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/JsonXYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Debug" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes/platform" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Comms" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Quadtree" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Watchdog_SAMD21" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1/src" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/libraries/SPI" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


