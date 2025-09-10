#!/bin/bash

src="/home/richard/Videos"
dest="/media/richard/EVO_500GB/coopcam"

while [ 1 ]
do
    printf "\nStoring any incoming videos...\n"
    
    success="false"

    if [ ! -z "$(ls -A ${src} )" ]; # Check that there are videos
    then

        if [ -d ${dest} ];
        then
            printf "Storing in ${dest}...\n"
            sudo rsync --progress ${src}/*.mp4 ${dest}
            success="true"
        else
            printf "Storage not available :(\n"
        fi
    fi

    if [ ${success} == "true" ];
    then
        printf "Deleting ${src}...\n"
        sudo rm -f ${src}/*.mp4
    fi
    sleep 3600
done

