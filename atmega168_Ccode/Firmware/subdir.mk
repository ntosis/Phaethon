################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/HeatingSystem -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/i2c_soft -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/timerInterrupt -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/RotaryEncoderButton -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/SPI -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/adt7301 -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/i2c -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/ds1307 -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/tm1637 -I/home/unix/MyCODE/Eclipse/workspace/Phaethon_Ccode/PID -Wall -O2 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega168p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


