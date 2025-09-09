#!/bin/bash

repo_name="pi-stuff/rpi02w"

printf "\nWould you like to install pir-cam? (Y/n):"
read install_pircam
printf "\n"
if [ "${install_pircam}" != "n" ]; then
#    printf "\nInstalling motion...\n"
#    sudo apt install motion -y
#    sudo cp ${HOME}/Programming/${repo_name}/motion.conf /etc/motion/
    
    printf "\nInstalling pigpio...\n"
    cd ${HOME}/Programs
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    rm -rf master.zip
    cd pigpio-master
    make
    sudo make install

#    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
#    sudo apt install raspberrypi-kernel-headers build-essential git -y
#    cd ${HOME}/Programs
#    git clone https://github.com/lwfinger/rtw88
#    cd rtw88
#    sudo dkms install $PWD
#    sudo make install_fw

    printf "\nBuilding IR light program...\n"
    cp ${HOME}/Programming/${repo_name}/let-there-be-light.c ${HOME}/Programs
    cd ${HOME}/Programs
    gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt

#    printf "\nCopying remote sending file(s)...\n"
#    cp ${HOME}/Programming/${repo_name}/send-files.sh ${HOME}/Programs/
#    chmod +x $HOME/Programs/send-files.sh
fi

sudo rm /etc/xdg/autostart/configure-pi02w-02.desktop 
sudo cp ${HOME}/Programming/${repo_name}/configure-pi02w-05.desktop /etc/xdg/autostart/

sudo reboot
