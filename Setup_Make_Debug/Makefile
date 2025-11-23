# # Toolchain
# CC = arm-none-eabi-gcc
# OBJCOPY = arm-none-eabi-objcopy
# GDB = arm-none-eabi-gdb

# # MCU & build flags
# MCU = cortex-m4
# FPU = fpv4-sp-d16
# FLOAT-ABI = hard
# CFLAGS = -Wall -O0 -g -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI) -std=gnu11
# LDFLAGS = -Tlinker_script.ld -nostartfiles

# # Sources & objects
# SRCS = main.c startup_stm32f446xx.s
# OBJS = $(SRCS:.c=.o)

# # Output
# TARGET = main.elf

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# %.o: %.s
# 	$(CC) -c $< -o $@

# flash: $(TARGET)
# 	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(TARGET) verify reset exit"

# debug: $(TARGET)
# 	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
# 	sleep 2
# 	$(GDB) $(TARGET) -ex "target extended-remote localhost:3333"

# clean:
# 	rm -f *.o *.elf

###############################################################################
# 🔧 Toolchain Setup
###############################################################################
CC       = arm-none-eabi-gcc
AS       = arm-none-eabi-as
OBJCOPY  = arm-none-eabi-objcopy
OBJDUMP  = arm-none-eabi-objdump
GDB      = arm-none-eabi-gdb
RM       = rm -rf

###############################################################################
# ⚙️ Project Configuration
###############################################################################
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

###############################################################################
# 🧩 Source and Include Files
###############################################################################
# Automatically find all .c and .s files
SRCS := $(wildcard $(SRC_DIR)/*.c)
ASMS := $(wildcard $(SRC_DIR)/*.s)

# Convert to object files in build directory
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS)) \
        $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(ASMS))

###############################################################################
# 🏗️ Compiler and Linker Flags
###############################################################################
CFLAGS  = -Wall -O0 -g -std=gnu11
CFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)
CFLAGS += -I$(INC_DIR)

LDFLAGS = -T$(LD_SCRIPT) -nostartfiles
LDFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)

###############################################################################
# 🎨 Terminal Colors
###############################################################################
YELLOW = \033[1;33m
GREEN  = \033[1;32m
BLUE   = \033[1;34m
RED    = \033[1;31m
RESET  = \033[0m

###############################################################################
# 🧱 Build Targets
###############################################################################
.PHONY: all clean flash debug info

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
	@echo "$(GREEN)✅ Build complete!$(RESET)"

# Link objects into final ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "$(BLUE)🔗 Linking...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "$(YELLOW)🧩 Compiling $<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@echo "$(YELLOW)⚙️  Assembling $<$(RESET)"
	@$(CC) -c $< -o $@

# Make build directory if not exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

###############################################################################
# 📦 Output formats (BIN + HEX)
###############################################################################
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O binary $< $@
	@echo "$(GREEN)📦 Created binary: $@$(RESET)"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O ihex $< $@
	@echo "$(GREEN)📦 Created hex: $@$(RESET)"

###############################################################################
# 🧹 Clean
###############################################################################
clean:
	@echo "$(RED)🧹 Cleaning build files...$(RESET)"
	@$(RM) $(BUILD_DIR)
	@echo "$(GREEN)✅ Clean done.$(RESET)"

###############################################################################
# ⚡ Flash (via OpenOCD)
###############################################################################
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

###############################################################################
# 🐞 Debug (OpenOCD + GDB)
###############################################################################
debug: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
	sleep 2
	$(GDB) $(BUILD_DIR)/$(TARGET).elf -ex "target extended-remote localhost:3333"

###############################################################################
# 🧠 Info (for developer insight)
###############################################################################
info:
	@echo "$(BLUE)🔍 Source files:$(RESET) $(SRCS)"
	@echo "$(BLUE)🔍 Object files:$(RESET) $(OBJS)"
	@echo "$(BLUE)🔍 Includes:$(RESET) $(INC_DIR)"
