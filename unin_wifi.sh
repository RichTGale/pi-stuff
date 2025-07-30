#!/bin/bash

sudo dkms remove rtw88/0.6 --all
sudo rm -rf /var/lib/dkms/rtw88
sudo rm -rf /usr/src/rtw88-0.6
sudo rm -f /etc/modprobe.d/rtw88.conf
