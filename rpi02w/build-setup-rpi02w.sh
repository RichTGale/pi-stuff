#!/bin/bash

printf "gcc -o setup-rpi02w ../../mycutils/mycutils.h ../../clocks/timer_sec/timer_sec.h ../../mycutils/mycutils.c ../../clocks/timer_sec.c setup-rpi02w.c\n"
gcc -o setup-rpi02w ../../mycutils/mycutils.h ../../clocks/timer_sec/timer_sec.h ../../mycutils/mycutils.c ../../clocks/timer_sec.c setup-rpi02w.c
