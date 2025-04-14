################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/usb_dbg/myudb_usb.c \
../common/usb_dbg/myudb_usbdesc.c 

C_DEPS += \
./common/usb_dbg/myudb_usb.d \
./common/usb_dbg/myudb_usbdesc.d 

OBJS += \
./common/usb_dbg/myudb_usb.o \
./common/usb_dbg/myudb_usbdesc.o 


# Each subdirectory must supply rules for building sources it contributes
common/usb_dbg/%.o: ../common/usb_dbg/%.c common/usb_dbg/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcpu=d25f -mcmodel=medium -Og -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Wall  -g -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers\reg_include" -I"C:\Users\SwaminathanK\Desktop\Thesis\DSP Library\Andes DSP\include" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\flash" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common\calibration" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\common" -I"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\vendor\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -mext-dsp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-common-2f-usb_dbg

clean-common-2f-usb_dbg:
	-$(RM) ./common/usb_dbg/myudb_usb.d ./common/usb_dbg/myudb_usb.o ./common/usb_dbg/myudb_usbdesc.d ./common/usb_dbg/myudb_usbdesc.o

.PHONY: clean-common-2f-usb_dbg

