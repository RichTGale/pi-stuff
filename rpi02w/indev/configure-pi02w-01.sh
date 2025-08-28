#!/bin/bash

printf "\nWould you like to change the amount of space allocated to the swapfile? (Y/n): "
read change_swap
printf "\n"
if [ "$change_swap" != "n" ]; then
    printf "\nOpening the swap config file for editing...\n"
    sudo dphys-swapfile swapoff
    sudo vim /etc/dphys-swapfile
    sudo dphys-swapfile setup
    sudo dphys-swapfile swapon
fi

sudo rm /etc/xdg/autostart/configure-pi02w-01.desktop 
sudo cp $HOME/Programming/pi-stuff/configure-pi02w-02.desktop /etc/xdg/autostart/

sudo reboot
