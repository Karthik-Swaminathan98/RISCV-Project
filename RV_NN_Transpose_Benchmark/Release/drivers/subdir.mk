################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/adc.c \
../drivers/aes.c \
../drivers/analog.c \
../drivers/audio.c \
../drivers/clock.c \
../drivers/emi.c \
../drivers/flash.c \
../drivers/gpio.c \
../drivers/i2c.c \
../drivers/lpc.c \
../drivers/mdec.c \
../drivers/plmt.c \
../drivers/pwm.c \
../drivers/s7816.c \
../drivers/spi.c \
../drivers/stimer.c \
../drivers/timer.c \
../drivers/uart.c \
../drivers/usbhw.c 

OBJS += \
./drivers/adc.o \
./drivers/aes.o \
./drivers/analog.o \
./drivers/audio.o \
./drivers/clock.o \
./drivers/emi.o \
./drivers/flash.o \
./drivers/gpio.o \
./drivers/i2c.o \
./drivers/lpc.o \
./drivers/mdec.o \
./drivers/plmt.o \
./drivers/pwm.o \
./drivers/s7816.o \
./drivers/spi.o \
./drivers/stimer.o \
./drivers/timer.o \
./drivers/uart.o \
./drivers/usbhw.o 

C_DEPS += \
./drivers/adc.d \
./drivers/aes.d \
./drivers/analog.d \
./drivers/audio.d \
./drivers/clock.d \
./drivers/emi.d \
./drivers/flash.d \
./drivers/gpio.d \
./drivers/i2c.d \
./drivers/lpc.d \
./drivers/mdec.d \
./drivers/plmt.d \
./drivers/pwm.d \
./drivers/s7816.d \
./drivers/spi.d \
./drivers/stimer.d \
./drivers/timer.d \
./drivers/uart.d \
./drivers/usbhw.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/DSP/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/NN/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/Core/Include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Transpose_Benchmark/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -O3 -mcmodel=medium -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -c -fmessage-length=0 -Wl,--gc-sections -fshort-wchar -fno-strict-aliasing -fomit-frame-pointer -fshort-enums -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


