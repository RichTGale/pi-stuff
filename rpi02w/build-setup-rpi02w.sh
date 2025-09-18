#!/bin/bash

printf "gcc -o setup-rpi02w utils.h timer_sec.h utils.c timer_sec.c setup-rpi02w.c\n"
gcc -o setup-rpi02w utils.h timer_sec.h utils.c timer_sec.c setup-rpi02w.c
