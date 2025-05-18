################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/flash/flash_mid146085.c \
../drivers/flash/flash_mid156085.c \
../drivers/flash/flash_mid166085.c 

OBJS += \
./drivers/flash/flash_mid146085.o \
./drivers/flash/flash_mid156085.o \
./drivers/flash/flash_mid166085.o 

C_DEPS += \
./drivers/flash/flash_mid146085.d \
./drivers/flash/flash_mid156085.d \
./drivers/flash/flash_mid166085.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/flash/%.o: ../drivers/flash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


