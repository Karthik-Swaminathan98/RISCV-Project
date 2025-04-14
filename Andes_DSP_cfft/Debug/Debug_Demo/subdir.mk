################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug_Demo/app.c \
../Debug_Demo/main.c 

OBJS += \
./Debug_Demo/app.o \
./Debug_Demo/main.o 

C_DEPS += \
./Debug_Demo/app.d \
./Debug_Demo/main.d 


# Each subdirectory must supply rules for building sources it contributes
Debug_Demo/%.o: ../Debug_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


