#!/bin/bash
# pico-sdk needs to be installed before running this script 
#
# Install dependencies 
# sudo apt install git build-essential pkg-config libusb-1.0-0-dev cmake
#

mkdir -p temp

cd temp; 

git clone https://github.com/raspberrypi/picotool.git;

cd picotool;

mkdir build; 
cd build; 
cmake ..;
make; 

# update permissions 
sudo cp udev/60-picotool.rules /etc/udev/rules.d/;

# install picotool
sudo make install;

cd ../../..; 

# clean up 
rm -rf temp 