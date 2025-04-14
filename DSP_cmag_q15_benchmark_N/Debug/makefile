################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifndef ANDESIGHT_ROOT
ANDESIGHT_ROOT=/cygdrive/C/Users/SwaminathanK/Downloads/Telink_IDE
endif

ifndef CROSS_COMPILE
CROSS_COMPILE=riscv32-elf-
endif

ifndef SECONDARY_OUTPUT_PATH
SECONDARY_OUTPUT_PATH=output
endif

$(shell mkdir -p $(SECONDARY_OUTPUT_PATH))

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Debug_Demo/subdir.mk
-include boot/subdir.mk
-include common/bt_debug/subdir.mk
-include common/subdir.mk
-include common/usb_dbg/subdir.mk
-include drivers/subdir.mk
-include drivers/flash/subdir.mk
-include vendor/common/auto_test/subdir.mk
-include vendor/common/calibration/subdir.mk
-include vendor/common/subdir.mk
-include vendor/common/flash/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
BUILD_ARTIFACT_FILE_BASE_NAME = DSP_cmag_q15_benchmark_N

LINKER_OUTPUTS = DSP_cmag_q15_benchmark_N.adx

GPROF_OUT += \
../gmon.sum ../gmon.out \

PWR_OUT += \
$(SECONDARY_OUTPUT_PATH)/pwr \

GCOV_OUT += \
$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)) \

SYMBOL_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/symbol.txt \

READELF_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/readelf.txt \

OBJDUMP_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/objdump.txt \

OBJCOPY_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin \

SIZE_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/.PHONY.size \


# All Target
all: DSP_cmag_q15_benchmark_N.adx secondary-outputs

# Tool invocations
DSP_cmag_q15_benchmark_N.adx: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Andes C Linker'
	$(CROSS_COMPILE)gcc -O2 -nostartfiles -static -L/cygdrive/C/Users/SwaminathanK/Downloads/ANDE/toolchains/nds32le-elf-mculib-v5f/riscv32-elf/lib/mext-dsp/mno-fma -L"/cygdrive/C/Users/SwaminathanK/AndeSight3/workspace/DSP_cmag_q15_benchmark_N/drivers/lib" -flto -mcmodel=medium -g -fno-jump-tables -T"/cygdrive/C/Users/SwaminathanK/Downloads/B91 SDK/chip/B91/link/flash_boot_ramcode.link" -Wl,-Map -Wl,$(SECONDARY_OUTPUT_PATH)/map.txt -Wl,--gc-sections -mext-dsp -ldsp -lm -fpack-struct -fshort-enums -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -fmessage-length=0 -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-gnu-zero-variadic-macro-arguments  -O3 -o "DSP_cmag_q15_benchmark_N.adx" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)): DSP_cmag_q15_benchmark_N.adx $(OBJS) $(USER_OBJS)
../gmon.sum ../gmon.out: DSP_cmag_q15_benchmark_N.adx $(OBJS) $(USER_OBJS)
$(SECONDARY_OUTPUT_PATH)/pwr: DSP_cmag_q15_benchmark_N.adx $(OBJS) $(USER_OBJS)

$(SECONDARY_OUTPUT_PATH)/symbol.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: NM (symbol listing)'
	$(CROSS_COMPILE)nm -n -l -C "DSP_cmag_q15_benchmark_N.adx" > $(SECONDARY_OUTPUT_PATH)/symbol.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/readelf.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Readelf (ELF info listing)'
	$(CROSS_COMPILE)readelf -a "DSP_cmag_q15_benchmark_N.adx" > $(SECONDARY_OUTPUT_PATH)/readelf.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/objdump.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objdump (disassembly)'
	$(CROSS_COMPILE)objdump -x -d -C "DSP_cmag_q15_benchmark_N.adx" > $(SECONDARY_OUTPUT_PATH)/objdump.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objcopy (object content copy)'
	$(CROSS_COMPILE)objcopy -S -O binary "DSP_cmag_q15_benchmark_N.adx" $(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/.PHONY.size: $(LINKER_OUTPUTS)
	@echo 'Invoking: Size (section size listing)'
	$(CROSS_COMPILE)size  "DSP_cmag_q15_benchmark_N.adx" | tee $(SECONDARY_OUTPUT_PATH)/.PHONY.size
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GPROF_OUT)$(PWR_OUT)$(SIZE_OUTPUTS)$(READELF_OUTPUTS)$(EXECUTABLES)$(OBJDUMP_OUTPUTS)$(OBJS)$(SYMBOL_OUTPUTS)$(S_UPPER_DEPS)$(GCOV_OUT)$(C_DEPS)$(OBJCOPY_OUTPUTS) DSP_cmag_q15_benchmark_N.adx
	-@echo ' '

secondary-outputs: $(SYMBOL_OUTPUTS) $(READELF_OUTPUTS) $(OBJDUMP_OUTPUTS) $(OBJCOPY_OUTPUTS) $(SIZE_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
