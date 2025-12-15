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
        -c "log_output regs.txt" \
        -c "mww 0xE000EDF4 0x10000" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10001" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10002" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10003" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10004" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10005" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10006" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10007" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10008" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x10009" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000A" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000B" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000C" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000D" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000E" -c "mdw 0xE000EDF8" \
        -c "mww 0xE000EDF4 0x1000F" -c "mdw 0xE000EDF8" \
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