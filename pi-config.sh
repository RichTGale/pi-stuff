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
sudo apt install vim git zsh dkms build-essential cmake unzip openvpn libelf-dev linux-headers-$(uname -r) -y

printf "\nConfiguring vim...\n"
cd $HOME/Programming
curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
cp $HOME/Programming/pi-stuff/.vimrc $HOME/
vim -c PlugInstall

printf "\nConfiguriing zsh...\n"
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
cd $HOME/Programming
cp $HOME/Programming/pi-stuff/.zshrc $HOME/
printf "\nSwitching default shell to zsh...\n"
chsh -s $(which zsh)

printf "\nWould you like to install pir-cam? (Y/n):"
read install_pircam
printf "\n"
if [ "$install_pircam" != "n" ]; then
    printf "\nInstalling motion...\n"
    sudo apt install motion -y
    
    printf "\nInstalling pigpio...\n"
    cd $HOME/Programs
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    rm -rf master.zip
    cd pigpio-master
    make
    sudo make install
    
    printf "\nInstalling pir-cam...\n"
    sudo cp $HOME/Programming/pi-stuff/motion.conf /etc/motion/
    sudo cp $HOME/Programming/pi-stuff/config.txt /boot/firmware
    cp $HOME/Programming/pi-stuff/let-there-be-light.c $HOME/Programs
    cd $HOME/Programs
    gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt
    
    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
    sudo apt install raspberrypi-kernel-headers build-essential git -y
    cd $HOME/Programs
    git clone https://github.com/lwfinger/rtw88
    cd rtw88
    sudo dkms install $PWD
    sudo make rtw8812a_fw

    printf "\nCopying autostart files...\n" #Need to edit this and do openvpn seperately
    cp $HOME/Programming/pi-stuff/my-autostart-apps.sh $HOME/Programs
    chmod +x $HOME/Programs/my-autostart-apps.sh
    sudo cp $HOME/Programming/pi-stuff/my-autostart-apps.desktop /etc/xdg/autostart/
    cp $HOME/Programming/pi-stuff/send-files.sh $HOME/Programs/
    chmod +x $HOME/Programs/send-files.sh
fi

if [ "$install_picam" == "n" ]; then
	printf "\nWould you like to install the RTL8821AU TP-Link Archer T2u Plus wifi driver? (Y/n): "
	read install_wifi
	printf "\n"
	if [ "$install_wifi" != "n" ]; then
	    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
	    sudo apt install raspberrypi-kernel-headers build-essential git -y
	    cd $HOME/Programs
	    git clone https://github.com/lwfinger/rtw88
	    cd rtw88
	    sudo dkms install $PWD
	    sudo make rtw8812a_fw
    fi
    printf "\nWould you like to install pigpio (https://github.com/joan2937/pigpio)? It is a C library for programming the computer's GPIO pins. (Y/n): "
	read install_pigpio
	printf "\n"
	if [ "$install_pigpio" != "n" ]; then
	    printf "\nInstalling pigpio...\n"
	    cd $HOME/Programs
	    wget https://github.com/joan2937/pigpio/archive/master.zip
	    unzip master.zip
	    rm -rf master.zip
	    cd pigpio-master
	    make
	    sudo make install
	fi
fi

printf "\nWould you like to change the amount of space allocated to the swapfile? (Y/n): "
read change_swap
printf "\n"
if [ "$change_swap" != "n" ]; then
    printf "\nOpening the swap config file for editing...\n"
    sleep 2
    sudo dphys-swapfile swapoff
    sudo vim /etc/dphys-swapfile
    sudo dphys-swapfile setup
    sudo dphys-swapfile swapon
fi

printf "\nWould you like to install SDL2? SDL2 is a C library for programming graphics and other media (Y/n): "
read install_sdl2
printf "\n"
if [ "$install_sdl2" != "n" ]; then
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

printf "\nThe script has finished. Press ENTER to quit...\n"
read pressEnter
