################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/libraries/SPI/SPI.cpp 

LINK_OBJ += \
./libraries/SPI/SPI.cpp.o 

CPP_DEPS += \
./libraries/SPI/SPI.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/SPI/SPI.cpp.o: /home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/libraries/SPI/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10609 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.0.0-atmel/CMSIS/Include/" "-I/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.0.0-atmel/Device/ATMEL/"  -I"/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/cores/arduino" -I"/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/variants/arduino_zero" -I"/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/libraries/SPI" -I"/home/carvajavo/Arduino/libraries/Adafruit_FONA" -I"/home/carvajavo/sloeber/arduinoPlugin/libraries/SD/1.1.1" -I"/home/carvajavo/sloeber/arduinoPlugin/libraries/SD/1.1.1/src" -I"/home/carvajavo/Documents/Github/Remora/libraries/Quadtree" -I"/home/carvajavo/Arduino/libraries/ESP8266_XYZ" -I"/home/carvajavo/Arduino/libraries/Quadtree" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


