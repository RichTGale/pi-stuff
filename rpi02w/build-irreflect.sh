#!/bin/bash

printf "gcc -o irreflect utils.h timer_sec.h utils.c timer_sec.c irreflect.c\n"
gcc -o irreflect utils.h timer_sec.h utils.c timer_sec.c irreflect.c
