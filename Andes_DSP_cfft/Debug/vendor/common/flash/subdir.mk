################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/flash/puya_common.c \
../vendor/common/flash/puya_flash_scratch.c \
../vendor/common/flash/puya_flash_trim.c 

OBJS += \
./vendor/common/flash/puya_common.o \
./vendor/common/flash/puya_flash_scratch.o \
./vendor/common/flash/puya_flash_trim.o 

C_DEPS += \
./vendor/common/flash/puya_common.d \
./vendor/common/flash/puya_flash_scratch.d \
./vendor/common/flash/puya_flash_trim.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/flash/%.o: ../vendor/common/flash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


