################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
SPI_ze_strony_diody.lss \

FLASH_IMAGE += \
SPI_ze_strony_diody.hex \

EEPROM_IMAGE += \
SPI_ze_strony_diody.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: SPI_ze_strony_diody.elf secondary-outputs

# Tool invocations
SPI_ze_strony_diody.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,SPI_ze_strony_diody.map -mmcu=atmega8 -o "SPI_ze_strony_diody.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

SPI_ze_strony_diody.lss: SPI_ze_strony_diody.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S SPI_ze_strony_diody.elf  >"SPI_ze_strony_diody.lss"
	@echo 'Finished building: $@'
	@echo ' '

SPI_ze_strony_diody.hex: SPI_ze_strony_diody.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex SPI_ze_strony_diody.elf  "SPI_ze_strony_diody.hex"
	@echo 'Finished building: $@'
	@echo ' '

SPI_ze_strony_diody.eep: SPI_ze_strony_diody.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex SPI_ze_strony_diody.elf  "SPI_ze_strony_diody.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: SPI_ze_strony_diody.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 SPI_ze_strony_diody.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) SPI_ze_strony_diody.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
