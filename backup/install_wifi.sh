#!/bin/bash

sudo apt update && sudo apt upgrade
sudo apt install -y raspberrypi-kernel-headers build-essential git
git clone https://github.com/lwfinger/rtw88
cd rtw88
sudo dkms install $PWD
sudo make install_fw
