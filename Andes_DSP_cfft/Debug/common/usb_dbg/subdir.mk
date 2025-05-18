################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/usb_dbg/myudb_usb.c \
../common/usb_dbg/myudb_usbdesc.c 

OBJS += \
./common/usb_dbg/myudb_usb.o \
./common/usb_dbg/myudb_usbdesc.o 

C_DEPS += \
./common/usb_dbg/myudb_usb.d \
./common/usb_dbg/myudb_usbdesc.d 


# Each subdirectory must supply rules for building sources it contributes
common/usb_dbg/%.o: ../common/usb_dbg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -Og -mcmodel=medium -g3 -Wall -mcpu=d25f -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


