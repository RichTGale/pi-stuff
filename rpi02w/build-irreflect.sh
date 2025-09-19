#!/bin/bash

printf "gcc -Wall -pthread -o irreflect utils.h timer_sec.h utils.c timer_sec.c irreflect.c -lpigpio -lrt\n"

gcc -Wall -pthread -o irreflect utils.h timer_sec.h utils.c timer_sec.c irreflect.c -lpigpio -lrt
