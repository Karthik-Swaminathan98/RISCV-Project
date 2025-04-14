################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/common/flash/subdir.mk
-include vendor/common/calibration/subdir.mk
-include vendor/common/auto_test/subdir.mk
-include vendor/common/subdir.mk
-include drivers/flash/subdir.mk
-include drivers/subdir.mk
-include common/usb_dbg/subdir.mk
-include common/bt_debug/subdir.mk
-include common/subdir.mk
-include boot/subdir.mk
-include Debug_Demo/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Debug
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Debug.bin \

SECONDARY_LIST += \
Debug.lst \

SECONDARY_SIZE += \
Debug.siz \


# All Target
all: main-build

# Main-build Target
main-build: Debug.elf secondary-outputs

# Tool invocations
Debug.elf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv32-elf-gcc -mcpu=d25f -mcmodel=medium -Og -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Wall  -g -T "C:\Users\SwaminathanK\Downloads\B91 SDK\chip\B91\link\flash_boot_ramcode.link" -nostartfiles -Xlinker --gc-sections -L"C:\Users\SwaminathanK\AndeSight3\workspace\DSP_cfft_q15_benchmark_N\drivers\lib" -L"C:\Users\SwaminathanK\Downloads\ANDE\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\lib\mext-dsp\mno-fma" -Wl,-Map,"Debug.map" -fpack-struct -fshort-enums -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -fmessage-length=0 -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-gnu-zero-variadic-macro-arguments  -O3 -mcmodel=medium -g3 -fno-jump-tables  -O0 -o "Debug.elf" $(OBJS) $(USER_OBJS) $(LIBS)  -lm -ldsp -ldriver -laxon_driver_lib
	@echo 'Finished building target: $@'
	@echo ' '

Debug.bin: Debug.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv32-elf-objcopy -O binary "Debug.elf"  "Debug.bin"
	@echo 'Finished building: $@'
	@echo ' '

Debug.lst: Debug.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv32-elf-objdump --source --all-headers --demangle --line-numbers --wide "Debug.elf" > "Debug.lst"
	@echo 'Finished building: $@'
	@echo ' '

Debug.siz: Debug.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv32-elf-size "Debug.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Debug.bin Debug.elf Debug.lst Debug.siz
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
