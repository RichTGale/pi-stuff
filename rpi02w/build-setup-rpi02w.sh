#!/bin/bash

printf "gcc -o setup-rpi02w utils.h utils.c setup-rpi02w.c\n"
gcc -o setup-rpi02w utils.h utils.c setup-rpi02w.c
