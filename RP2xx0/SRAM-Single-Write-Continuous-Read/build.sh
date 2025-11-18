#!/bin/bash

RP2040_SRAM_TO_USE_B=$((200*1000))
RP2350_SRAM_TO_USE_B=$((400*1000)) 

clang-format -i --style=Google *.cpp
echo "Formatted source files"

# # build for RP2040
# mkdir -p build_2040
# cd build_2040; 
# cmake -DPICO_SDK_PATH=$(pwd)/../../pico-sdk \
#       -DCMAKE_CXX_FLAGS="-DSRAM_TO_USE_B=$RP2040_SRAM_TO_USE_B" \
#       -DPICO_PLATFORM=rp2040 \
#       -DPICO_BOARD=pico \
#       ..;
# make;

# mkdir -p ../_uf2/
# cp *.uf2 ../_uf2/sram_single_write_continuous_read_rp2040.uf2

# echo "Built for RP2040"
# cd ..;

# build for RP2350 (ARM)
mkdir -p build_2350_arm
cd build_2350_arm; 
cmake -DPICO_SDK_PATH=$(pwd)/../../pico-sdk \
      -DCMAKE_CXX_FLAGS="-DSRAM_TO_USE_B=$RP2040_SRAM_TO_USE_B" \
      -DPICO_PLATFORM=rp2350 \
      -DPICO_BOARD=pico2 \
      ..;
make;

cp *.uf2 ../_uf2/sram_single_write_continuous_read_rp2350_arm.uf2
echo "Built for RP2350 (ARM cores)"
cd ..;

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