################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../../../../demo/vendor/Debug_Demo/app.c \
../../../../demo/vendor/Debug_Demo/main.c 

OBJS += \
./vendor/Debug_Demo/app.o \
./vendor/Debug_Demo/main.o 

C_DEPS += \
./vendor/Debug_Demo/app.d \
./vendor/Debug_Demo/main.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/Debug_Demo/app.o: ../../../../demo/vendor/Debug_Demo/app.c vendor/Debug_Demo/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\chip\B91\drivers" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\common" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Debug_Demo/main.o: ../../../../demo/vendor/Debug_Demo/main.c vendor/Debug_Demo/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\chip\B91\drivers" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\common" -I"C:\Users\SwaminathanK\Downloads\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


