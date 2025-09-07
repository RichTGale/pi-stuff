#!/bin/bash

src="/home/richard/Videos"
dest="/media/richard/EVO_500GB/coopcam"

printf "dest = ${dest}\n"

while [ 1 ]
do
    printf "\nStoring any incoming videos...\n"
    
    success="false"

    if [ ! -z "$(ls -A ${src} )" ]; # Check that there are videos
    then

        if [ ! -d ${dest} ];
        then
            mkdir ${dest}
        else
            printf "Storing in ${dest}...\n"
            sudo rsync --progress ${src}/*.mp4 ${dest}
            success="true"
        fi
    fi

    if [ ${success} == "true" ];
    then
        printf "Deleting ${src}...\n"
        sudo rm -f ${src}/*.mp4
    fi
    sleep 60
done

