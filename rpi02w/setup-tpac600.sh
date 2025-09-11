#!/bin/bash

printf "\nWould you like to install the RTL8821AU TP-Link Archer T2u Plus wifi driver? (Y/n): "
read install_wifi
printf "\n"
if [ "${install_wifi}" != "n" ]; then
    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
    sudo apt update; sudo apt upgrade -y
    sudo apt install -y raspberrypi-kernel-headers build-essential git
    cd ${HOME}/Programs
    git clone https://github.com/lwfinger/rtw88
    cd rtw88
    sudo dkms install $PWD
    sudo make install_fw
fi
