#!/bin/bash

printf "\nCopying config files...\n"
sudo cp $HOME/Programming/pi-stuff/config.txt /boot/firmware/

sudo rm /etc/xdg/autostart/configure-pi02w-05.desktop 
sudo cp $HOME/Programming/pi-stuff/my-autostart-apps.desktop /etc/xdg/autostart/

sudo reboot
