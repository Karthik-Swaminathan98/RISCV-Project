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


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcpu=d25f -mcmodel=medium -Og -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Wall  -g -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers\reg_include" -I"C:\Users\SwaminathanK\Desktop\Thesis\DSP Library\Andes DSP\include" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\flash" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\calibration" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -mext-dsp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/adc.d ./drivers/adc.o ./drivers/aes.d ./drivers/aes.o ./drivers/analog.d ./drivers/analog.o ./drivers/audio.d ./drivers/audio.o ./drivers/clock.d ./drivers/clock.o ./drivers/emi.d ./drivers/emi.o ./drivers/flash.d ./drivers/flash.o ./drivers/gpio.d ./drivers/gpio.o ./drivers/i2c.d ./drivers/i2c.o ./drivers/lpc.d ./drivers/lpc.o ./drivers/mdec.d ./drivers/mdec.o ./drivers/plmt.d ./drivers/plmt.o ./drivers/pwm.d ./drivers/pwm.o ./drivers/s7816.d ./drivers/s7816.o ./drivers/spi.d ./drivers/spi.o ./drivers/stimer.d ./drivers/stimer.o ./drivers/timer.d ./drivers/timer.o ./drivers/uart.d ./drivers/uart.o ./drivers/usbhw.d ./drivers/usbhw.o

.PHONY: clean-drivers

