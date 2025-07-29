#!/bin/bash

nmcli device disconnect wlan0
sleep 30
nmcli device connect wlan1
sleep 30
printf "Restarting VPN service. Network config will display once the VPN has restarted.\n"
sudo systemctl restart openvpn-client@client.service
sleep 30
ip a
sudo motion
$HOME/Programs/let-there-be-light

#echo Press any button to end...
#read pressEnter
