################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/cores/arduino/avr/dtostrf.c 

C_DEPS += \
./core/dtostrf.c.d 

AR_OBJ += \
./core/dtostrf.c.o 


# Each subdirectory must supply rules for building sources it contributes
core/dtostrf.c.o: /home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/cores/arduino/avr/dtostrf.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-gcc" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -DF_CPU=48000000L -DARDUINO=10609 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.0.0-atmel/CMSIS/Include/" "-I/home/carvajavo/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.0.0-atmel/Device/ATMEL/"  -I"/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/cores/arduino" -I"/home/carvajavo/sloeber/arduinoPlugin/packages/adafruit/hardware/samd/1.0.13/variants/arduino_zero" -I"/home/carvajavo/Arduino/libraries/Quadtree" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


