################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../boot/cstartup_flash.S \
../boot/cstartup_ram.S 

OBJS += \
./boot/cstartup_flash.o \
./boot/cstartup_ram.o 

S_UPPER_DEPS += \
./boot/cstartup_flash.d \
./boot/cstartup_ram.d 


# Each subdirectory must supply rules for building sources it contributes
boot/%.o: ../boot/%.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv32-elf-gcc -mcpu=d25f -mcmodel=medium -Og -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Wall  -g -x assembler-with-cpp -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\flash" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\calibration" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers\reg_include" -I"C:\Users\SwaminathanK\Desktop\Thesis\DSP Library\Andes DSP\include" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\flash" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\calibration" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-boot

clean-boot:
	-$(RM) ./boot/cstartup_flash.d ./boot/cstartup_flash.o ./boot/cstartup_ram.d ./boot/cstartup_ram.o

.PHONY: clean-boot

