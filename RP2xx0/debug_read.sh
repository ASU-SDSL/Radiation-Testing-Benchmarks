#!/bin/bash

if [[ -z "$1" || -z "$2" ]]; then 
    echo "Usage: debug_read.sh <target-device> <output-name>"
    echo "  target-device: rp2040 | rp2350"
    echo "  output-name: name for the output file(s)"
    exit 1
fi

echo "Flashing target device $1..."

# read registers in order R0-R12, SP, LR, PC

if [[ "$1" = "rp2040" ]]; then 
    sudo openocd -f interface/cmsis-dap.cfg -f debug/rp2040.cfg \
        -c "adapter speed 5000" \
        -c "init" -c "targets rp2040.core0" -c "halt" \
        -c "dump_image out.bin 0x20000000 0x42000" \
        -c "resume" -c "shutdown"
elif [[ "$1" = "rp2350" ]]; then
    sudo openocd -f interface/cmsis-dap.cfg -f debug/rp2350.cfg \
        -c "adapter speed 5000" \
        -c "init" -c "targets rp2040.core0" -c "halt" \
        -c "dump_image out.bin 0x20000000 0x80000" \
        -c "resume" -c "shutdown"
else 
    echo "Unsupported target device: $1"
    exit 1
fi

mkdir -p readbacks 
mv out.bin readbacks/${1}_${2}.bin

echo "Done."