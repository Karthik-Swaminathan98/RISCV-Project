################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/auto_test/pc_interface.c 

OBJS += \
./vendor/common/auto_test/pc_interface.o 

C_DEPS += \
./vendor/common/auto_test/pc_interface.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/auto_test/%.o: ../vendor/common/auto_test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/Andes DSP/include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_fir_q15/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -O3 -mcmodel=medium -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


