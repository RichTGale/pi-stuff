#!/bin/bash

src="/home/richard/Videos"
dest="/media/richard/EVO_500GB/coopcam"

#sudo umount /media/richard/EVO_500GB
#sudo umount /media/richard/EVO_500GB1
#sudo mount --source UUID=6EFF-BE49 /home/richard/EVO_500GB

printf "dest = ${dest}\n"

while [ 1 ]
do
    printf "\nStoring any incoming videos...\n"
    
    success="false"

    ls ${src}
    if [ "${?}" == "0" ];
    then

        sudo ls ${dest}
        if [ $? == 0 ];
        then
            printf "Storing in ${dest}...\n"
            success="true"
            sudo rsync --progress ${src}/*.mp4 ${dest}
        fi
    fi

    if [ ${success} == "true" ];
    then
        printf "Deleting ${src}...\n"
        sudo rm -f ${src}/*.mp4
    fi
    sleep 60
done
