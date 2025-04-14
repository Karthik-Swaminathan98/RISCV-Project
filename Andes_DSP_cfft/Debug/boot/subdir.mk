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
boot/%.o: ../boot/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


