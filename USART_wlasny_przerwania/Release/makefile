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
USART_wlasny_przerwania.lss \

FLASH_IMAGE += \
USART_wlasny_przerwania.hex \

EEPROM_IMAGE += \
USART_wlasny_przerwania.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: USART_wlasny_przerwania.elf secondary-outputs

# Tool invocations
USART_wlasny_przerwania.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,USART_wlasny_przerwania.map -mmcu=atmega8 -o "USART_wlasny_przerwania.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

USART_wlasny_przerwania.lss: USART_wlasny_przerwania.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S USART_wlasny_przerwania.elf  >"USART_wlasny_przerwania.lss"
	@echo 'Finished building: $@'
	@echo ' '

USART_wlasny_przerwania.hex: USART_wlasny_przerwania.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex USART_wlasny_przerwania.elf  "USART_wlasny_przerwania.hex"
	@echo 'Finished building: $@'
	@echo ' '

USART_wlasny_przerwania.eep: USART_wlasny_przerwania.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex USART_wlasny_przerwania.elf  "USART_wlasny_przerwania.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: USART_wlasny_przerwania.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 USART_wlasny_przerwania.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) USART_wlasny_przerwania.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
