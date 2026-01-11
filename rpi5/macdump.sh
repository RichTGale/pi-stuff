#!/bin/bash

install_aircrack="n"
using_waln1="n"

printf "Would you like to install aircrack-ng? This will update your system and install aircrack-ng and macchanger. <y/N>: "
read install_aircrack
printf "\n"

printf "Are you using wlan1 (tp link ac600 wifi aerial)? <y/N>: "
read using_wlan1
printf "\n"

if [ "${install_aircrack}" == "y" ];
then
  sudo apt update ; sudo apt upgrade ; sudo apt install aircrack-ng macchanger
fi

sudo airmon-ng check kill

if [ "${using_wlan1}" == "y" ];
then
  sudo ip link set wlan0 down
  sleep 2
  sudo ip link set wlan1 up
  sudo airmon-ng start wlan1
  sleep 2
  sudo ip link set wlan1mon down
  sudo macchanger -r wlan1mon
  sudo ip link set wlan1mon up
  sudo airodump-ng --write ${HOME}/Documents/dump --output-format csv wlan1
else
  printf "Monitor mode not supported on this wireless interface\n"
fi

read pressEnter

