#!/bin/bash

printf "g++ -Wall -pthread -o ltbl ltbl.c -lpigpio -lrt\n"

gcc -Wall -pthread -o ltbl ltbl.c -lpigpio -lrt
