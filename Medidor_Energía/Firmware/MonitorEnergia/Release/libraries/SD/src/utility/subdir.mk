################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/Sd2Card.cpp \
/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/SdFile.cpp \
/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/SdVolume.cpp 

LINK_OBJ += \
./libraries/SD/src/utility/Sd2Card.cpp.o \
./libraries/SD/src/utility/SdFile.cpp.o \
./libraries/SD/src/utility/SdVolume.cpp.o 

CPP_DEPS += \
./libraries/SD/src/utility/Sd2Card.cpp.d \
./libraries/SD/src/utility/SdFile.cpp.d \
./libraries/SD/src/utility/SdVolume.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/SD/src/utility/Sd2Card.cpp.o: /home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/Sd2Card.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/cores/arduino" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/variants/arduino_zero" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/SD/src/utility/SdFile.cpp.o: /home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/SdFile.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/cores/arduino" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/variants/arduino_zero" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/SD/src/utility/SdVolume.cpp.o: /home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src/utility/SdVolume.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++" -mcpu=cortex-m0plus -mthumb -c -g -Os -Wall -Wextra -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -DF_CPU=48000000L -DARDUINO=10802 -DARDUINO_SAMD_FEATHER_M0 -DARDUINO_ARCH_SAMD -DARDUINO_SAMD_ZERO -D__SAMD21G18A__ -DUSB_VID=0x239A -DUSB_PID=0x800B -DUSBCON '-DUSB_MANUFACTURER="Adafruit"' '-DUSB_PRODUCT="Feather M0"' "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/" "-I/home/bryanmtdt/Documents/sloeber4.1//arduinoPlugin/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/"  -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/cores/arduino" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/variants/arduino_zero" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/libraries/SD/1.1.1/src" -I"/home/bryanmtdt/Documents/sloeber4.1/arduinoPlugin/packages/adafruit/hardware/samd/1.0.17/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


