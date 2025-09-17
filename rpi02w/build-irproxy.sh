#!/bin/bash

printf "g++ -Wall -pthread -o sensor  timer_nano.h timer_nano.c main.c -lpigpio -lrt\n"

g++ -Wall -pthread -o sensor timer_nano.h timer_nano.c irproxy.c -lpigpio -lrt

