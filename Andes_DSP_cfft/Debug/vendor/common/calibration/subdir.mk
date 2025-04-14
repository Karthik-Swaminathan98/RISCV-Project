################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/calibration/calibration.c 

OBJS += \
./vendor/common/calibration/calibration.o 

C_DEPS += \
./vendor/common/calibration/calibration.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/calibration/%.o: ../vendor/common/calibration/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


