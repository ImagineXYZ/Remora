################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0/variant.cpp 

AR_OBJ += \
./core/variant/variant.cpp.o 

CPP_DEPS += \
./core/variant/variant.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
core/variant/variant.cpp.o: /home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0/variant.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/cores/arduino" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Persistence" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/ESP8266_XYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/JsonXYZ" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Debug" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes/platform" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA/includes" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Adafruit_FONA" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Comms" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Quadtree" -I"/home/sergio/Documentos/sloeber-workspace/Remora/libraries/Watchdog_SAMD21" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1" -I"/home/sergio/sloeber/arduinoPlugin/libraries/SD/1.2.1/src" -I"/home/sergio/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


