#1/bin/bash

printf "gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt\n"
gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt
