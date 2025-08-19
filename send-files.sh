#!/bin/bash

host="/home/richard/Videos"
remote1="10.8.0.4:/home/richard/Videos"
remote2="root@10.8.0.1:/root/Videos"

# Loop indefinitely
while [ 1 ]
do
    sleep 1
    if [ ! -z "$(ls -A $host )" ]; # Check that there are videos
    then
        printf "A video was recorded...\n"
        sleep 15
        videos=($host/*)   # Get the array of detections
        v=0
        for v in "${!videos[@]}"; do
            
            ping -c 1 -w 2 10.8.0.4
            if [ "$?" == "0" ];
            then
                printf "Sending video to $remote1...\n"
                scp ${videos[v]} $remote1
            fi
            
            ping -c 1 -w 2 10.8.0.1
            if [ "$?" == "0" ];
            then
                printf "Sending video to $remote2...\n"
                scp ${videos[v]} $remote2
            fi
            mv ${videos[v]} /home/richard/Programs/videos
            sleep 15
        done
#        printf "Deleting local video(s)...\n"
#        sudo rm -f $host/*
    fi
done
