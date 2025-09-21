#!/bin/bash

install_aircrack="n"
using_waln1="n"

printf "Would you like to install aircrack-ng? <y/N>: "
read install_aircrack
printf "\n"

printf "Are you using wlan1 (tp link ac600 wifi aerial)? <y/N>: "
read using_wlan1
printf "\n"

if [ "${install_aircrack}" != "n" ];
then
  sudo apt update ; sudo apt upgrade ; sudo apt install aircrack-ng
fi

sudo airmon-ng check kill

if [ "${using wlan1}" != "n" ];
then
  sudo ip link set wlan0 down
  sleep 2
  sudo ip link set wlan1 up
  sudo airmon-ng start wlan1
  sleep 2
  sudo ip link set wlan1 down
  sudo macchanger -r wlan1
  sudo ip link set wlan1 up
  sudo airodump-ng --write ${HOME}/Documents/dump --output-format csv wlan1
else
  sudo airmon-ng start wlan0
  sleep 2
  sudo ip link set wlan0 down
  sudo macchanger -r wlan0
  sudo ip link set wlan0 up
  sudo airodump-ng --write ${HOME}/Documents/dump --output-format csv wlan0
fi

read pressEnter

