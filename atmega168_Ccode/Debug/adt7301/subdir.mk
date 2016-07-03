################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adt7301/temperatureSens.c 

OBJS += \
./adt7301/temperatureSens.o 

C_DEPS += \
./adt7301/temperatureSens.d 


# Each subdirectory must supply rules for building sources it contributes
adt7301/%.o: ../adt7301/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/i2c -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/ds1307 -Wall -g2 -gstabs -O2 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega168p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


