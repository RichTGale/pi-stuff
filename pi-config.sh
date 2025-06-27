#!/bin/bash

printf "\nCreating $HOME/Programming and $HOME/Programs directories to install stuff in later...\n"
if [ ! -d $HOME/Programming ]; then
    mkdir $HOME/Programming
fi

if [ ! -d $HOME/Programs ]; then
    mkdir $HOME/Programs
fi

printf "\nDoing a full system update and upgrade...\n"
sudo apt update
sudo apt full-upgrade -y

printf "\nInstalling some utilities and dependencies...\n"
sudo apt install vim git zsh dkms build-essential cmake unzip libelf-dev linux-headers-$(uname -r) -y

printf "\nConfiguring vim...\n"
cd $HOME/Programming
curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
cp $HOME/Programming/pi5-config/.vimrc $HOME/
vim -c PlugInstall

printf "\nConfiguriing zsh...\n"
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
cd $HOME/Programming
cp $HOME/Programming/pi5-config/.zshrc $HOME/
printf "\nSwitching default shell to zsh...\n"
chsh -s $(which zsh)

printf "\nWould you like to install the RTL8821AU TP-Link Archer T2u Plus wifi driver? (Y/n): "
read response
printf "\n"
if [ "$response" != "n" ]; then
    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
    cd $HOME/Programs
    git clone https://github.com/aircrack-ng/rtl8812au.git
    cd rtl8812au
    sudo make dkms_install
fi

printf "\nWould you like to install pigpio (https://github.com/joan2937/pigpio)? It is a C library for programming the computer's GPIO pins. (Y/n): "
read response
printf "\n"
if [ "$response" != "n" ]; then
    printf "\nInstalling pigpio...\n"
    cd $HOME/Programs
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    rm -rf master.zip
    cd pigpio-master
    make
    sudo make install
fi

printf "\nWould you like to change the amount of space allocated to the swapfile? (Y/n): "
read response
printf "\n"
if [ "$response" != "n" ]; then
    printf "\nOpening the swap config file for editing...\n"
    sleep 2
    sudo dphys-swapfile swapoff
    sudo vim /etc/dphys-swapfile
    sudo dphys-swapfile setup
    sudo dphys-swapfile swapon
fi

printf "\nWould you like to install SDL2? SDL2 is a C library for programming graphics and other media (Y/n): "
read response
printf "\n"
if [ "$response" != "n" ]; then
    printf "\nInstalling SDL2...\n"
    sudo apt install build-essential git make curl jq pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev fcitx-libs-dev -y

    cd $HOME/Programs
    version=$(curl -sL https://api.github.com/repos/libsdl-org/SDL/releases/latest | jq -r '.name')
    wget $(curl -sL https://api.github.com/repos/libsdl-org/SDL/releases/latest | jq -r ".assets[].browser_download_url" | grep -m 1 "SDL2-${version}.tar.gz")
    tar -xf "SDL2-${version}.tar.gz"
    cd "SDL2-${version}"
    mkdir build
    cd build
    ../configure
    make
    sudo make install

    cd $HOME/Programs
    version=$(curl -sL https://api.github.com/repos/libsdl-org/SDL_ttf/releases/latest | jq -r '.name')
    wget $(curl -sL https://api.github.com/repos/libsdl-org/SDL_ttf/releases/latest | jq -r ".assets[].browser_download_url" | grep -m 1 "SDL2_ttf-${version}.tar.gz")
    tar -xf "SDL2_ttf-${version}.tar.gz"
    cd "SDL2_ttf-${version}"
    mkdir build
    cd build
    ../configure
    make
    sudo make install
fi

printf "\nCleaning up...\n"
sudo apt autoremove -y

printf "\nThe script has finished. Press ENTER to reboot...\n"
read pressEnter

sudo reboot
