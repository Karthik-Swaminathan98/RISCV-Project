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
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


