#!/bin/bash

if [[ -z "$1" || -z "$2" ]]; then 
    echo "Usage: debug_read.sh <target-device> <output-name>"
    echo "  target-device: rp2040 | rp2350"
    echo "  output-name: name for the output file(s)"
    exit 1
fi

echo "Flashing target device $1..."

if [[ "$1" = "rp2040" ]]; then 
    sudo openocd -f interface/cmsis-dap.cfg -f debug/rp2040.cfg \
        -c "adapter speed 5000" \
        -c "init" -c "targets rp2040.core0" -c "halt" \
        -c "mdw 0xE000ED00 1" \
        -c "mdw 0xE000EDF0 1" \
        -c "mdw 0x20000000 1" \
        -c "reg" \
        -c "log_output default" \
        -c "dump_image out.bin 0x20000000 0x42000" \
        -c "resume" -c "shutdown"
elif [[ "$1" = "rp2350" ]]; then
    sudo openocd -f interface/cmsis-dap.cfg -f debug/rp2350.cfg -c "adapter speed 5000" -c "dump_image out.bin 0x10000000 0x200000 exit"
else 
    echo "Unsupported target device: $1"
    exit 1
fi

echo "Done."