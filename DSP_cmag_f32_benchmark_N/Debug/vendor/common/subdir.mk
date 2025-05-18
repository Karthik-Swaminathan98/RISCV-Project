################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/common.c \
../vendor/common/exception.c \
../vendor/common/printf.c 

OBJS += \
./vendor/common/common.o \
./vendor/common/exception.o \
./vendor/common/printf.o 

C_DEPS += \
./vendor/common/common.d \
./vendor/common/exception.d \
./vendor/common/printf.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/%.o: ../vendor/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/Andes DSP/include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cmag_f32_benchmark_N/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -Og -mcmodel=medium -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


