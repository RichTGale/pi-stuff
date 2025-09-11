#!/bin/bash

#nmcli device disconnect wlan0
#sleep 5
nmcli device connect wlan1
sleep 5

#printf "Restarting VPN service. Network config will display once the VPN has restarted.\n"
#sudo systemctl restart openvpn-client@client.service
#sleep 5

lxterminal -e "sudo ${HOME}/Programs/reconnect.sh"
sleep 5

#lxterminal -e "sudo ${HOME}/Programs/send-files.sh"
#sleep 5
#
#lxterminal -e "sudo ${HOME}/Programs/sensor -t 24"
#sleep 5
#
#lxterminal -e "sudo ${HOME}/Programs/store-files.sh"
#sleep 5
#
#lxterminal -e "sudo ${HOME}/Programs/macdump.sh"
#sleep 2

sudo motion

ip a
sleep 2

echo Press any button to end...
read pressEnter
