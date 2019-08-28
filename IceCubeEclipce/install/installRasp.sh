#! /bin/bash
sudo apt update
sudo apt upgrade
sudo apt install python -y python2.7 python3 -y
sudo apt install default-jre -y default-jdk -y
sudo apt install gcc make cmake dkms build-essential libusb-1.0.0-dev
sudo apt install gcc-arm-none-eabi stm32flash
sudo apt install avrdude binutils-avr gcc-avr avr-libc gdb-avr
sudo apt install git net-tools ht cutecom putty
mkdir toolchain
wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2
tar -xvf gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2 
mv gcc-arm-none-eabi-8-2019-q3-update/ toolchain/
rm gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2
sudo mv toolchain/ /opt/