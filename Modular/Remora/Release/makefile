################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries/Watchdog_SAMD21/subdir.mk
-include libraries/SPI/subdir.mk
-include libraries/SD/src/utility/subdir.mk
-include libraries/SD/src/subdir.mk
-include libraries/Quadtree/subdir.mk
-include libraries/Persistence/subdir.mk
-include libraries/JsonXYZ/subdir.mk
-include libraries/ESP8266_XYZ/subdir.mk
-include libraries/Comms/subdir.mk
-include libraries/Adafruit_FONA/subdir.mk
-include core/variant/subdir.mk
-include core/core/avr/subdir.mk
-include core/core/USB/subdir.mk
-include core/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/home/sergio/Documentos/sloeber-workspace/Remora/Release/arduino.ar \

ELF += \
Remora.elf \

HEX += \
Remora.hex \


# All Target
all: Remora

# Tool invocations
Remora: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-size" -A "/home/sergio/Documentos/sloeber-workspace/Remora/Release/Remora.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/home/sergio/Documentos/sloeber-workspace/Remora/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-ar" rcs    /home/sergio/Documentos/sloeber-workspace/Remora/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

Remora.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-gcc" "-L/home/sergio/Documentos/sloeber-workspace/Remora/Release" -Os -Wl,--gc-sections -save-temps "-T/home/sergio/sloeber//arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0/linker_scripts/gcc/flash_with_bootloader.ld" "-Wl,-Map,/home/sergio/Documentos/sloeber-workspace/Remora/Release/Remora.map" --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m0plus -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -o "/home/sergio/Documentos/sloeber-workspace/Remora/Release/Remora.elf"   $(LINK_OBJ)  -Wl,--start-group "-L/home/sergio/sloeber//arduinoPlugin/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Lib/GCC/" -larm_cortexM0l_math "-L/home/sergio/sloeber//arduinoPlugin/packages/adafruit/hardware/samd/1.0.21/variants/feather_m0" -lm  /home/sergio/Documentos/sloeber-workspace/Remora/Release/arduino.ar  -Wl,--end-group
	@echo 'Finished building: $@'
	@echo ' '

Remora.hex: $(ELF)
	@echo 'Do all objcopy commands'
	${A.RECIPE.OBJCOPY.BIN.PATTERN}
	${A.RECIPE.OBJCOPY.HEX.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
