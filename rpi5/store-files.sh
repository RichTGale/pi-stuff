#!/bin/bash

src="/home/richard/Videos"
tar="/media/richard/EVO_500GB/coopcam"

printf "Storing any received videos...\n"

# Loop indefinitely
while [ 1 ]
do
    if [ ! -z "$(ls -A ${src})" ]; # Check that there are videos
    then
        printf "Videos were received...\n"
        vids=(${src}/*)   # Get the array of videos
        vid=0
        for v in "${!vids[@]}"; do

            if [ -d ${tar} ];
            then
                printf "Sending video to ${tar}...\n"
                rsync --progress ${vids[vid]} ${tar}
                sudo rm -f rm ${vids[vid]}
            else
                printf "${tar} isn't accessible :(\n"
            fi
        done
    fi
    sleep 1800  # Wait 30 minutes
done
