################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../i2c_soft/i2c_soft.c 

OBJS += \
./i2c_soft/i2c_soft.o 

C_DEPS += \
./i2c_soft/i2c_soft.d 


# Each subdirectory must supply rules for building sources it contributes
i2c_soft/%.o: ../i2c_soft/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/adt7301 -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/tm1637 -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/PID -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/RotaryEncoderButton -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/HeatingSystem -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/i2c -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/timerInterrupt -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/SPI -I/home/unix/Arduino/Thermostat_RottaryButton/Phaethon/atmega168_Ccode/ds1307 -Wall -g2 -gstabs -O2 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega168p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


