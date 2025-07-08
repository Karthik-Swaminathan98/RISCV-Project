################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug_Demo/app.c \
../Debug_Demo/main.c \
../Debug_Demo/test_riscv_softmax_s16.c \
../Debug_Demo/test_riscv_softmax_s8.c \
../Debug_Demo/test_riscv_softmax_s8_s16.c \
../Debug_Demo/utility.c 

OBJS += \
./Debug_Demo/app.o \
./Debug_Demo/main.o \
./Debug_Demo/test_riscv_softmax_s16.o \
./Debug_Demo/test_riscv_softmax_s8.o \
./Debug_Demo/test_riscv_softmax_s8_s16.o \
./Debug_Demo/utility.o 

C_DEPS += \
./Debug_Demo/app.d \
./Debug_Demo/main.d \
./Debug_Demo/test_riscv_softmax_s16.d \
./Debug_Demo/test_riscv_softmax_s8.d \
./Debug_Demo/test_riscv_softmax_s8_s16.d \
./Debug_Demo/utility.d 


# Each subdirectory must supply rules for building sources it contributes
Debug_Demo/%.o: ../Debug_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/DSP/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/NN/Include" -I"/cygdrive/C/Users/SwaminathanK/Desktop/Thesis/DSP Library/NMSIS-master/NMSIS/NMSIS/Core/Include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/drivers" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/drivers/reg_include" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/vendor/common/flash" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/vendor/common/calibration" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/common" -I"/cygdrive/C/Users/SwaminathanK/Workspace-Ande3/RV_NN_Softmax_Benchmark/vendor/common" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/lib/gcc/riscv32-elf/7.4.0/include" -I"/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE/ide//RDS/V3.2.3/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/include" -O3 -mcmodel=medium -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -c -fmessage-length=0 -Wl,--gc-sections -fshort-wchar -fno-strict-aliasing -fomit-frame-pointer -fshort-enums -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


