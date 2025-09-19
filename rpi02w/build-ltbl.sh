#!/bin/bash

printf "gcc -Wall -pthread -o ltbl utils.h timer_sec.h utils.c timer_sec.c ltbl.c -lpigpio -lrt\n"

gcc -Wall -pthread -o ltbl utils.h timer_sec.h utils.c timer_sec.c ltbl.c -lpigpio -lrt
