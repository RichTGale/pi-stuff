#!/bin/bash

src="/home/richard/Videos"                  # source directory
tar1="/media/richard/EVO_500GB/coopcam"      # target directory
tar2="/media/richard/EVO_500GB1/coopcam"      # target directory

wait_time=1800

printf "Storing any received videos...\n"

# Loop indefinitely
while [ 1 ]
do
    if [ ! -z "$(ls -A ${src})" ]; # Check that there are videos
    then
        printf "Videos were received...\n"
        vids=(${src}/*)   # Get the array of videos
        vid=0
        for vid in "${!vids[@]}"; do
            if [ -d ${tar1} ];
            then
                vlc --play-and-exit ${vids[vid]}
                printf "Sending video to ${tar1}...\n"
                sudo rsync --progress ${vids[vid]} ${tar1}
                sudo rm -f rm ${vids[vid]}
            elif [ -d ${tar2} ];
            then
                vlc --play-and-exit ${vids[vid]}
                printf "Sending video to ${tar2}...\n"
                sudo rsync --progress ${vids[vid]} ${tar2}
                sudo rm -f rm ${vids[vid]}
            else 
                printf "${tar1} and ${tar2} arn't accessible :(\n"
            fi
        done
    fi
    sleep ${wait_time} 
done
