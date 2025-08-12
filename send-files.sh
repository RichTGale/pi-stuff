#!/bin/bash

host="/home/richard/Videos"
remote1="10.8.0.4:/home/richard/Videos"
remote2="root@10.8.0.1:/root/Videos"

# Loop indefinitely
while [ 1 ]
do
    if [ ! -z "$(ls -A $host )" ]; # Check that there are videos
    then
        printf "A video was recorded...\n"
        sleep 60
        videos=($host/*)   # Get the array of detections
        v=0
        for v in "${!videos[@]}"; do
            printf "Sending video to client(s)...\n"
            scp ${videos[v]} $remote2
            scp ${videos[v]} $remote1
            sleep 60
        done
        printf "Deleting local video(s)...\n"
        sudo rm -f $host/*
    fi
done
