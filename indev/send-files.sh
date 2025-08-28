#!/bin/bash

users=( "richard" )
dir="Videos"
local_locales=( 192.168.0.84 )
remote_locales=( 10.8.0.1 10.8.0.4  )
#remote1="richard@10.8.0.4:/home/richard/Videos"
#remote2="root@10.8.0.1:/root/Videos"

# Loop indefinitely
while [ 1 ]
do
#    sleep 30
    if [ ! -z "$(ls -A $host )" ]; # Check that there are videos
    then
        printf "A video was recorded...\n"
        sleep 15 # Give the video time to record
        u=0
        for u in "${!users[@]}";
        do
            videos=("/home/${users[u]}/${dir}/*")   # Get the potential array of videos
            v=0
	        for v in "${!videos[@]}";
	        do
	            # Attempt to send to a local device
	            sent_locally=0
	            for rl in "${!local_locales[@]}";
	            do
	                ping -c 1 -w 2 ${local_locales[rl]}
	                if [ "$?" == "0" ];
	                then
	                    printf "Sending video to ${local_locales[rl]}...\n"
	                    scp ${videos[v]} "${users[u]}@${local_locales[rl]}/home/${users[u]}/${dir}"
	                    if [ $? == "0" ];
	                    then
	                        $sent_locally=1
	                    fi
	                fi
	            done
	
	            # Attempt to send to a local device
	            sent_remotely=0
	            for rg in "${!remote_locales[@]}";
	            do
	                ping -c 1 -w 2 ${remote_locales[rl]}
	                if [ "$?" == "0" ];
	                then
	                    printf "Sending video to ${remote_locales[rl]}...\n"
	                    scp ${videos[v]} "${users[u]}@${remote_locales[rg]}/home/${users[u]}/${dir}"
	                    if [ $? == "0" ];
	                    then
	                        $sent_remotely=1
	                    fi
	                fi
	            done
                printf "Moving video...\n"
                mv ${videos[v]} /home/${users[u]}/Programs/videos
                printf "\n"
            done

            printf "\n"

            sleep 15
        done
    fi
done
