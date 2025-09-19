#!/bin/bash

printf "gcc -Wall -pthread -o irreflect ../../mycutils/mycutils.h ../../clocks/timer_sec/timer_sec.h ../../mycutils/mycutils.c ../../clocks/timer_sec/timer_sec.c irreflect.c -lpigpio -lrt\n"
gcc -Wall -pthread -o irreflect ../../mycutils/mycutils.h ../../clocks/timer_sec/timer_sec.h ../../mycutils/mycutils.c ../../clocks/timer_sec/timer_sec.c irreflect.c -lpigpio -lrt
