#!/bin/bash



printf "Installing zerosec...\n"

printf "\nInstalling motion...\n"
sudo apt install motion -y
sudo cp ${device_path}/motion.conf /etc/motion/

printf "\nInstalling pigpio...\n"
cd ${HOME}/Programs
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
rm -rf master.zip
cd pigpio-master
make
sudo make install

printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
sudo apt install raspberrypi-kernel-headers build-essential git -y
cd ${HOME}/Programs
git clone https://github.com/lwfinger/rtw88
cd rtw88
sudo dkms install $PWD
sudo make install_fw

printf "\nBuilding IR light program...\n"
cp ${device_path}/let-there-be-light.c ${HOME}/Programs
cd ${HOME}/Programs
gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt

printf "\nCopying remote sending file(s)...\n"
cp ${device_path}/send-files.sh ${HOME}/Programs/
chmod +x $HOME/Programs/send-files.sh

sudo rm /etc/xdg/autostart/install-zerosec.desktop 
sudo cp ${HOME}/Programming/${repo_name}/finalise-setup.desktop /etc/xdg/autostart/

sudo reboot
