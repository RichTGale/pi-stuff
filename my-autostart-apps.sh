#!/bin/bash

nmcli device disconnect wlan0
sleep 30
nmcli device connect wlan1
sleep 30
sudo systemctl restart openvpn-client@client.service

#echo Press any button to end...
#read pressEnter
