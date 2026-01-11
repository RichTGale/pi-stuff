#!/bin/bash

sudo apt update ; sudo apt upgrade -y ; sudo apt install aircrack-ng macchanger -y

sudo airmon-ng check kill

sudo ip link set wlan0 down
sleep 2
sudo ip link set wlan1 up
sudo airmon-ng start wlan1
sleep 2
sudo ip link set wlan1mon down
sudo macchanger -r wlan1mon
sudo ip link set wlan1mon up
sudo airodump-ng --write ${HOME}/Documents/macdump --output-format csv wlan1mon

sudo airmon-ng stop wlan1mon

