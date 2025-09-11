#!/bin/bash

printf "g++ -Wall -pthread -o sensor  mycutils.h mycutils.c sensor.c -lpigpio -lrt\n"

gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt
