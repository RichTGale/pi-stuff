#!/bin/bash

host="/home/richard/Videos"
remote="10.8.0.3:/home/richard/Videos"

# Loop indefinitely
while [ 1 ]
do
    if [ ! -z "$(ls -A $host )" ]; # Check that there are videos
    then
        sleep 70
        videos=($host/*)   # Get the array of detections
        v=0
        for v in "${!videos[@]}"; do
            scp ${videos[v]} $remote
            sleep 70
        done
        sudo rm -f $host/*
    fi
done
