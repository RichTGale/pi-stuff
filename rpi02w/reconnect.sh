#!/bin/bash

wait_time=60

printf "VPN reconnect program has started...\n\n"

while [ 1 ]
do
    sleep ${wait_time}
    printf "Assertaning if you are connected to your VPN...\n"
    ping -c 2 -w -q 5 10.8.0.1
    if [ $? != "0" ];
    then
        printf "\nYou are not connected to your VPN... Attempting to reconnect...\n"
        sudo systemctl restart openvpn-client@client.service
        sleep 5
        
        ping -c 2 -w 5 -q 10.8.0.1
        if [ $? != "0" ];
        then
            printf "Failed to reconnect to your VPN :( Retrying in ${wait_time} seconds...\n\n"
        else
            printf "\nReconnection successful :)\n\n"
        fi
    else
        printf "\nYou are connected to your VPN :)\n\n"
    fi
done
