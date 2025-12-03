#!/bin/bash

RP2040_SRAM_TO_USE_B=$((200*1000))
RP2350_SRAM_TO_USE_B=$((400*1000)) 

if [[ -n "$1" && "$1" == "--help" ]]; then
    echo "Usage: build.sh [all|rp2040|rp2350-arm|rp2350-riscv]"
    echo "If no arguments are provided, builds for all platforms."
    exit 0
fi

clang-format -i --style=Google *.cpp
echo "Formatted source files"

# set up PICO_SDK_PATH and save old version 
OLD_PICO_SDK_PATH="$PICO_SDK_PATH"

export PICO_SDK_PATH="$(pwd)/../pico-sdk"

# determine what to build based on args

if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2040" ]]; then 
      # build for RP2040
      mkdir -p build_2040

      cmake -S . -B build_2040 \
            -DPICO_PLATFORM=rp2040 \
            -DPICO_BOARD=pico \
            -DSRAM_TO_USE_B=$RP2040_SRAM_TO_USE_B \
            ;
      cmake --build build_2040 -- -j$(nproc);

      mkdir -p _uf2
      cp build_2040/*.uf2 _uf2/sram_single_write_continuous_read_rp2040.uf2
      echo -e "Built for RP2040\n\n"

      WIDTH=$(tput cols)
      CHAR='-'
      LINE=$(printf '%*s' "${WIDTH}" '' | tr ' ' "${CHAR}")
      echo "$LINE"
fi

if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2350-arm" ]]; then 
      # build for RP2350 (ARM)
      mkdir -p build_2350_arm

      cmake -S . -B build_2350_arm \
            -DPICO_PLATFORM=rp2350-arm-s \
            -DPICO_BOARD=pico2 \
            -DSRAM_TO_USE_B=$RP2350_SRAM_TO_USE_B \
            ;
      cmake --build build_2350_arm -- -j$(nproc);

      mkdir -p _uf2
      cp build_2350_arm/*.uf2 _uf2/sram_single_write_continuous_read_rp2350_arm.uf2
      echo "Built for RP2350 (ARM cores)"

      WIDTH=$(tput cols)
      CHAR='-'
      LINE=$(printf '%*s' "${WIDTH}" '' | tr ' ' "${CHAR}")
      echo "$LINE"
fi

if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2350-riscv" ]]; then 
      # build for RP2350 (ARM)
      mkdir -p build_2350_riscv

      cmake -S . -B build_2350_riscv \
            -DPICO_PLATFORM=rp2350-riscv \
            -DPICO_BOARD=pico2 \
            -DSRAM_TO_USE_B=$RP2350_SRAM_TO_USE_B \
            ;
      cmake --build build_2350_riscv -- -j$(nproc);

      mkdir -p _uf2
      cp build_2350_riscv/*.uf2 _uf2/sram_single_write_continuous_read_rp2350_riscv.uf2
      echo "Built for RP2350 (RISC-V cores)"

      WIDTH=$(tput cols)
      CHAR='-'
      LINE=$(printf '%*s' "${WIDTH}" '' | tr ' ' "${CHAR}")
      echo "$LINE"
fi

# # build for RP2350 (RISC-V)
# mkdir -p build_2350_riscv
# cd build_2350_riscv; 
# cmake -DPICO_SDK_PATH=$(pwd)/../../pico-sdk \
#       -DCMAKE_CXX_FLAGS="-DSRAM_TO_USE_B=$RP2040_SRAM_TO_USE_B" \
#       -DPICO_PLATFORM=rp2350-riscv \
#       -DPICO_BOARD=pico2 \
#       ..;
# make;

# cp *.uf2 ../_uf2/sram_single_write_continuous_read_rp2350_riscv.uf2
# echo "Built for RP2350 (RISC-V cores)"
# cd ..;

# restore old PICO_SDK_PATH
if [ -n "$OLD_PICO_SDK_PATH" ]; then
    export PICO_SDK_PATH="$OLD_PICO_SDK_PATH"
else
    unset PICO_SDK_PATH
fi

# print final message and info 
echo "Finished build scripts for: "
if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2040" ]]; then 
    echo "- RP2040"
fi
if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2350-arm" ]]; then 
    echo "- RP2350 (ARM cores)"
fi
if [[ "$#" -eq 0 || $* =~ "all" || $* =~ "rp2350-riscv" ]]; then 
    echo "- RP2350 (RISC-V cores)"
fi