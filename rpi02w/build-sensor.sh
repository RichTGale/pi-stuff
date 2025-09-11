#!/bin/bash

printf "g++ -Wall -pthread -o sensor  mycutils.h mycutils.c main.c -lpigpio -lrt\n"

g++ -Wall -pthread -o sensor  mycutils.h mycutils.c sensor.c -lpigpio -lrt
