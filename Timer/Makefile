# Toolchain Setup
CC       = arm-none-eabi-gcc
AS       = arm-none-eabi-as
OBJCOPY  = arm-none-eabi-objcopy
OBJDUMP  = arm-none-eabi-objdump
SIZE     = arm-none-eabi-size
GDB      = arm-none-eabi-gdb
RM       = rm -rf

# Project Configuration
TARGET    = firmware

# MCU configuration (adjust per chip)
MCU       = cortex-m4
FPU       = fpv4-sp-d16
FLOAT-ABI = hard

# Paths
SRC_DIR   = Src
INC_DIR   = Inc
BUILD_DIR = build
LD_SCRIPT = linker_script.ld

# Version Configuration
# User-defined release version (change as needed)
VERSION   = MY_RELEASE_1.0.2

# Build Configuration (Debug / Release)
ifeq ($(DEBUG),1)
CFLAGS_OPT = -O0 -g
BUILD_TYPE = Debug
else ifeq ($(RELEASE),1)
CFLAGS_OPT = -O2
BUILD_TYPE = Release
else
CFLAGS_OPT = -O0 -g
BUILD_TYPE = Default(Debug)
endif

# Build Versioning (auto date/time + version tag)
BUILD_DATE := $(shell date +"%Y-%m-%d")
BUILD_TIME := $(shell date +"%H:%M:%S")
CFLAGS += -DBUILD_DATE=\"$(BUILD_DATE)\" -DBUILD_TIME=\"$(BUILD_TIME)\" -DBUILD_VERSION=\"$(VERSION)\"

# Source and Include Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
ASMS := $(wildcard $(SRC_DIR)/*.s)

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS)) \
        $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(ASMS))

# Compiler and Linker Flags
CFLAGS  += -Wall $(CFLAGS_OPT) -std=gnu11
CFLAGS  += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)
CFLAGS  += -I$(INC_DIR)

LDFLAGS = -T$(LD_SCRIPT) -nostartfiles
LDFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)

# Build Targets
.PHONY: all clean flash debug info

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
	@echo "---------------------------------------------------------"
	@echo "Build complete - Mode: $(BUILD_TYPE)"
	@echo "Build Version: $(VERSION)"
	@echo "Build Date: $(BUILD_DATE)  Time: $(BUILD_TIME)"
	@echo "---------------------------------------------------------"

# Link objects into final ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Created ELF: $@"
	@$(SIZE) --format=berkeley $@
	@echo "---------------------------------------------------------"
	@$(OBJDUMP) -h -S $@ > $(BUILD_DIR)/$(TARGET)_sections.txt
	@echo "Section details : $(BUILD_DIR)/$(TARGET)_sections.txt"

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@echo "Assembling $<"
	@$(CC) -c $< -o $@

# Make build directory if not exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Output formats (BIN + HEX)
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O binary $< $@
	@echo "Created binary  : $@"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O ihex $< $@
	@echo "Created hex     : $@"

# Clean
clean:
	@echo "Cleaning build files..."
	@$(RM) $(BUILD_DIR)
	@echo "Clean done."

# Flash (via OpenOCD)
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

# Debug (OpenOCD + GDB)
debug: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
	sleep 2
	$(GDB) $(BUILD_DIR)/$(TARGET).elf -ex "target extended-remote localhost:3333"

# Info
info:
	@echo "Source files : $(SRCS)"
	@echo "Object files : $(OBJS)"
	@echo "Include path : $(INC_DIR)"
	@echo "Build Mode   : $(BUILD_TYPE)"
	@echo "Build Date   : $(BUILD_DATE) $(BUILD_TIME)"
	@echo "Version Tag  : $(VERSION)"
