#!/bin/bash

nmcli device disconnect wlan0
sleep 15
nmcli device connect wlan1
sleep 15
printf "Restarting VPN service. Network config will display once the VPN has restarted.\n"
sudo systemctl restart openvpn-client@client.service
sleep 15
ip a
lxterminal --geometry=300x300 -e "sudo $HOME/Programs/let-there-be-light off"
sleep 10
lxterminal --geometry=300x300 -e "$HOME/Programs/send-files.sh"
sleep 10
sudo motion


echo Press any button to end...
read pressEnter
