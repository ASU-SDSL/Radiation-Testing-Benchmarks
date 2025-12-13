#!/bin/bash

if [[ -z "$1" || -z "$2" ]]; then 
    echo "Usage: debug_flash.sh <target-device> <path-to-elf-file>"
    echo "  target-device: rp2040 | rp2350"
    echo "  path-to-elf-file: Path to the .elf file to flash onto the device"
    exit 1
fi

echo "Flashing target device $1..."

if [[ "$1" = "rp2040" ]]; then 
    sudo openocd -f interface/cmsis-dap.cfg -f ../debug/rp2040.cfg -c "adapter speed 5000" -c "program $2 verify reset exit"
elif [[ "$1" = "rp2350" ]]; then
    sudo openocd -f interface/cmsis-dap.cfg -f ../debug/rp2350.cfg -c "adapter speed 5000" -c "program $2 verify reset exit"
else 
    echo "Unsupported target device: $1"
    exit 1
fi

echo "Done."