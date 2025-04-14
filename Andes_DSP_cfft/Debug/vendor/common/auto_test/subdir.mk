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
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


