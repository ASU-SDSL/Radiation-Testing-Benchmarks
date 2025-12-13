#!/bin/bash
BUILD_NAME_BASE="simple-usb"

function print_separator {
      WIDTH=$(tput cols)
      CHAR='-'
      LINE=$(printf '%*s' "${WIDTH}" '' | tr ' ' "${CHAR}")
      echo -e "\n$LINE\n"
}

function args_contain() {
      local target="$1"

      shift # discard local arg 

      if [[ "$#" -eq 0 || "$*" =~ "all" || "$*" =~ "$target" ]]; then 
            return 0
      else 
            return 1
      fi
}

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
if args_contain "rp2040" "$@"; then 
      # build for RP2040
      mkdir -p build_2040

      cmake -S . -B build_2040 \
            -DPICO_PLATFORM=rp2040 \
            -DPICO_BOARD=pico \
            ;
      cmake --build build_2040 -- -j$(nproc);

      mkdir -p _uf2
      cp build_2040/*.uf2 "_uf2/${BUILD_NAME_BASE}_rp2040.uf2"

      mkdir -p _elf 
      cp build_2040/*.elf "_elf/${BUILD_NAME_BASE}_rp2040.elf"

      echo -e "Built for RP2040"

      print_separator
fi

if args_contain "rp2350-arm" "$@"; then 
      # build for RP2350 (ARM)
      mkdir -p build_2350_arm

      cmake -S . -B build_2350_arm \
            -DPICO_PLATFORM=rp2350-arm-s \
            -DPICO_BOARD=pico2 \
            ;
      cmake --build build_2350_arm -- -j$(nproc);

      mkdir -p _uf2
      cp build_2350_arm/*.uf2 "_uf2/${BUILD_NAME_BASE}_rp2350_arm.uf2"

      mkdir -p _elf
      cp build_2350_arm/*.elf "_elf/${BUILD_NAME_BASE}_rp2350_arm.elf"

      echo "Built for RP2350 (ARM cores)"

      print_separator
fi

if args_contain "rp2350-riscv" "$@"; then 
      # build for RP2350 (RISC-V)
      mkdir -p build_2350_riscv

      cmake -S . -B build_2350_riscv \
            -DPICO_TOOLCHAIN_PATH=$(pwd)/../_tools/riscv/riscv-toolchain-15 \
            -DPICO_PLATFORM=rp2350-riscv \
            -DPICO_BOARD=pico2 \
            ;
      cmake --build build_2350_riscv -- -j$(nproc);

      mkdir -p _uf2
      cp build_2350_riscv/*.uf2 "_uf2/${BUILD_NAME_BASE}_rp2350_riscv.uf2"

      mkdir -p _elf
      cp build_2350_riscv/*.elf "_elf/${BUILD_NAME_BASE}_rp2350_riscv.elf"

      echo "Built for RP2350 (RISC-V cores)"

      print_separator
fi

# restore old PICO_SDK_PATH
if [ -n "$OLD_PICO_SDK_PATH" ]; then
    export PICO_SDK_PATH="$OLD_PICO_SDK_PATH"
else
    unset PICO_SDK_PATH
fi

# print final message and info 
echo "Finished running build scripts for: ${BUILD_NAME_BASE}"
if args_contain "rp2040" "$@"; then 
    echo "- RP2040"
fi
if args_contain "rp2350-arm" "$@"; then 
    echo "- RP2350 (ARM cores)"
fi
if args_contain "rp2350-riscv" "$@"; then 
    echo "- RP2350 (RISC-V cores)"
fi