################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/flash/flash_mid146085.c \
../drivers/flash/flash_mid156085.c \
../drivers/flash/flash_mid166085.c 

C_DEPS += \
./drivers/flash/flash_mid146085.d \
./drivers/flash/flash_mid156085.d \
./drivers/flash/flash_mid166085.d 

OBJS += \
./drivers/flash/flash_mid146085.o \
./drivers/flash/flash_mid156085.o \
./drivers/flash/flash_mid166085.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/flash/%.o: ../drivers/flash/%.c drivers/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcpu=d25f -mcmodel=medium -Og -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Wall  -g -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers\reg_include" -I"C:\Users\SwaminathanK\Desktop\Thesis\DSP Library\Andes DSP\include" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\flash" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\calibration" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -mext-dsp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers-2f-flash

clean-drivers-2f-flash:
	-$(RM) ./drivers/flash/flash_mid146085.d ./drivers/flash/flash_mid146085.o ./drivers/flash/flash_mid156085.d ./drivers/flash/flash_mid156085.o ./drivers/flash/flash_mid166085.d ./drivers/flash/flash_mid166085.o

.PHONY: clean-drivers-2f-flash

