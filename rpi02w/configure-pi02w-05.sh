#!/bin/bash

repo_name="pi-stuff/rpi02w"

printf "\nCopying config files...\n"
sudo cp ${HOME}/Programming/${repo_name}/config.txt /boot/firmware/

sudo rm /etc/xdg/autostart/configure-pi02w-05.desktop 
sudo cp ${HOME}/Programming/${repo_name}/my-autostart-apps.desktop /etc/xdg/autostart/

sudo reboot
