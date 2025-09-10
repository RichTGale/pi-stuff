#!/bin/bash



printf "\nCopying config files...\n"
sudo cp ${device_path}/config.txt /boot/firmware/

sudo rm /etc/xdg/autostart/finalise-setup.desktop 
sudo cp ${device_path}/autoapps.desktop /etc/xdg/autostart/

sudo reboot
