#!/bin/bash

sudo airmon-ng check kill
sleep 2
sudo ip link set wlan0 down
sleep 2
sudo ip link set wlan1 up
sleep 2
sudo airmon-ng start wlan1
sleep 2
sudo ip link set wlan1 down
sudo macchanger -r wlan1
sudo ip link set wlan1 up
sudo airodump-ng --write /home/richard/Documents/dump --output-format csv wlan1

read pressEnter

