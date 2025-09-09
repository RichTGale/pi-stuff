#!/bin/bash

while [ 1 ]
do
    sleep 60
    ping -c 2 -w 5 10.8.0.1
    if [ $? != "0" ];
    then
        printf "Not connected... reconnecting...\n"
        sudo systemctl restart openvpn-client@client.service
        sleep 5
        
        ping -c 2 -w 5 10.8.0.1
        if [ $? != "0" ];
        then
            printf "Failed to reconnect... retrying in 60 seconds...\n"
        fi
    else
        printf "Connected...\n"
    fi
done
