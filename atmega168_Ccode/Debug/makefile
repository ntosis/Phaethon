################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include u8glib/Debug/subdir.mk
-include tm1637/subdir.mk
-include timerInterrupt/subdir.mk
-include i2c/subdir.mk
-include ds1307/subdir.mk
-include adt7301/subdir.mk
-include SPI/subdir.mk
-include RotaryEncoderButton/subdir.mk
-include PID/subdir.mk
-include HeatingSystem/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Phaethon_Ccode.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: Phaethon_Ccode.elf secondary-outputs

# Tool invocations
Phaethon_Ccode.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Phaethon_Ccode.map -mmcu=atmega168p -o "Phaethon_Ccode.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Phaethon_Ccode.lss: Phaethon_Ccode.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Phaethon_Ccode.elf  >"Phaethon_Ccode.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Phaethon_Ccode.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega168p Phaethon_Ccode.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Phaethon_Ccode.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
