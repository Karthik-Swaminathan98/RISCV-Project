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
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/Andes DSP/include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/Andes_DSP_Math_benchmark/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -O3 -mcmodel=medium -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -c -fmessage-length=0 -Wl,--gc-sections -fshort-wchar -fno-strict-aliasing -fomit-frame-pointer -fshort-enums -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


