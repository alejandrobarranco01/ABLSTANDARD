#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
FLASH_ADDR="0x08000000"

echo ">>> STM32 Flash Script"

# Check dependencies
if ! command -v st-flash &> /dev/null; then
    echo "ERROR: st-flash not found. Install with: sudo apt install stlink-tools"
    exit 1
fi

if ! command -v arm-none-eabi-objcopy &> /dev/null; then
    echo "ERROR: arm-none-eabi-objcopy not found. Install with: sudo apt install binutils-arm-none-eabi"
    exit 1
fi

# Find the .elf file
ELF_FILE=$(find "$BUILD_DIR" -maxdepth 1 -name "*.elf" | head -1)

if [ -z "$ELF_FILE" ]; then
    echo "ERROR: No .elf file found in $BUILD_DIR"
    echo "Make sure you have built the project first with ./build.sh"
    exit 1
fi

# Convert .elf to .bin
BIN_FILE="${ELF_FILE%.elf}.bin"
echo ">>> Converting: $(basename "$ELF_FILE") -> $(basename "$BIN_FILE")"
arm-none-eabi-objcopy -O binary "$ELF_FILE" "$BIN_FILE"

# Flash
echo ">>> Flashing to $FLASH_ADDR..."
st-flash --reset write "$BIN_FILE" "$FLASH_ADDR"

echo ">>> Flash complete!"
