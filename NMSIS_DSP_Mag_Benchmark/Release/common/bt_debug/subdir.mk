################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/bt_debug/dbgport.c 

OBJS += \
./common/bt_debug/dbgport.o 

C_DEPS += \
./common/bt_debug/dbgport.d 


# Each subdirectory must supply rules for building sources it contributes
common/bt_debug/%.o: ../common/bt_debug/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/DSP/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/NN/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/Core/Include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/NMSIS_DSP_Mag_Benchmark/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -Os -mcmodel=medium -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -c -fmessage-length=0 -Wl,--gc-sections -fshort-wchar -fno-strict-aliasing -fomit-frame-pointer -fshort-enums -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


