#!/bin/bash

mkdir -p ../_tools

wget https://github.com/raspberrypi/pico-sdk-tools/releases/download/v2.2.0-2/riscv-toolchain-15-x86_64-lin.tar.gz
sudo mkdir -p ../_tools/riscv/riscv-toolchain-15
sudo chown $USER ../_tools/riscv/riscv-toolchain-15
tar xvf riscv-toolchain-15-x86_64-lin.tar.gz -C ../_tools/riscv/riscv-toolchain-15
rm -rf riscv-toolchain-15-x86_64-lin.tar.gz