################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/flash/flash_mid146085.c \
../drivers/flash/flash_mid156085.c \
../drivers/flash/flash_mid166085.c 

OBJS += \
./drivers/flash/flash_mid146085.o \
./drivers/flash/flash_mid156085.o \
./drivers/flash/flash_mid166085.o 

C_DEPS += \
./drivers/flash/flash_mid146085.d \
./drivers/flash/flash_mid156085.d \
./drivers/flash/flash_mid166085.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/flash/%.o: ../drivers/flash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/Andes DSP/include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/DSP_cfft_f32_stack_N/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -mext-dsp -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


