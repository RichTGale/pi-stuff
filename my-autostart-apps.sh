#!/bin/bash

sudo ip link set wlan0 down
sudo ip link set wlan1 up

sudo systemctl restart wpa_supplicant.service
sudo systemctl restart openvpn-client@client.service

echo Press any button to end...
read pressEnter
