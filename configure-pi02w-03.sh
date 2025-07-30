#!/bin/bash

printf "\nWould you like to install pigpio (https://github.com/joan2937/pigpio)? It is a C library for programming the computer's GPIO pins. (Y/n): "
read install_pigpio
printf "\n"
if [ "$install_pigpio" != "n" ]; then
    printf "\nInstalling pigpio...\n"
    cd $HOME/Programs
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    rm -rf master.zip
    cd pigpio-master
    make
    sudo make install
fi

printf "\nWould you like to install the RTL8821AU TP-Link Archer T2u Plus wifi driver? (Y/n): "
read install_wifi
printf "\n"
if [ "$install_wifi" != "n" ]; then
    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
    sudo apt update; sudo apt upgrade
    sudo apt install -y raspberrypi-kernel-headers build-essential git
    cd $HOME/Programs
    git clone https://github.com/lwfinger/rtw88
    cd rtw88
    sudo dkms install $PWD
    sudo make rtw8812a_fw
fi

sudo rm /etc/xdg/autostart/configure-pi02w-03.desktop 
sudo cp $HOME/Programming/pi-stuff/configure-pi02w-04.desktop /etc/xdg/autostart/

sudo reboot
