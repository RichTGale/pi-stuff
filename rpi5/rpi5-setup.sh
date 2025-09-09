#!/bin/bash

repo_name="rpisec/rpi5"

printf "\nCreating ${HOME}/Programming and ${HOME}/Programs directories to install stuff in later...\n"
if [ ! -d ${HOME}/Programming ]; then
    mkdir ${HOME}/Programming
fi

if [ ! -d ${HOME}/Programs ]; then
    mkdir ${HOME}/Programs
fi

printf "\nDoing a full system update and upgrade...\n"
sudo apt update
sudo apt full-upgrade -y

printf "\nInstalling some utilities and dependencies...\n"
sudo apt install vim git zsh dkms build-essential cmake make curl unzip macchanger aircrack-ng libelf-dev linux-headers-$(uname -r) -y

printf "\nWould you like to configure vim? (Y/n):"
read install_vim
printf "\n"
if [ "${install_vim}" != "n" ]; then
	printf "\nConfiguring vim...\n"
	cd ${HOME}/Programming
	curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	cp ${HOME}/Programming/"${repo_name}"/.vimrc ${HOME}
	vim -c PlugInstall
fi

printf "\nWould you like to configure zsh? (Y/n):"
read install_zsh
printf "\n"
if [ "${install_zsh}" != "n" ]; then
	printf "\nConfiguring zsh...\n"
	sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
	git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
	git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
	cd ${HOME}/Programming
	cp ${HOME}/Programming/"${repo_name}"/.zshrc ${HOME}
	printf "\nSwitching default shell to zsh...\n"
	chsh -s $(which zsh)
fi

printf "\nWould you like to install the RTL8821AU TP-Link Archer T2u Plus wifi driver? (Y/n): "
read install_wifi
printf "\n"
if [ "${install_wifi}" != "n" ]; then
    printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
    cd ${HOME}/Programs
    git clone https://github.com/aircrack-ng/rtl8812au.git
    cd rtl8812au
    sudo make dkms_install

    printf "\nCopying autostart files...\n"
    sudo cp ${HOME}/Programming/"${repo_name}"/my-autostart-apps.desktop /etc/xdg/autostart
    cp ${HOME}/Programming/"${repo_name}"/my-autostart-apps.sh ${HOME}/Programs
fi

printf "\nWould you like to change the amount of space allocated to the swapfile? (Y/n): "
read change_swap
printf "\n"
if [ "${change_swap}" != "n" ]; then
    printf "\nOpening the swap config file for editing...\n"
    sleep 2
    sudo dphys-swapfile swapoff
    sudo vim /etc/dphys-swapfile
    sudo dphys-swapfile setup
    sudo dphys-swapfile swapon
fi

printf "\nWould you like to install Steam? (Y/n): "
read install_steam
printf "\n"
if [ "${install_steam}" != "n" ]; then
    printf "\nBuilding and installing Steam dependencies...\n"
    sudo dpkg --add-architecture armhf
    sudo apt install git gcc-arm-linux-gnueabihf libgdm1:armhf libudev1:armhf libgl1-mesa-dri:armhf libglapi-mesa:armhf libglu1-mesa:armhf libglx-mesa0:armhf mesa-va-drivers:armhf mesa-vdpau-drivers:armhf mesa-vulkan-drivers:armhf libsdl1.2debian:armhf libegl-mesa0:armhf libc6:armhf mesa-vulkan-drivers -y

    printf "\nBuilding and installing box64...\n"
    cd ${HOME}/Programs
    git clone https://github.com/ptitSeb/box64.git
    cd box64
    mkdir build; cd build; cmake .. -D RPI5ARM64=1 -D CMAKE_BUILD_TYPE=RelWithDebInfo
    make -j4; sudo make install
    sudo systemctl restart systemd-binfmt

    printf "\nBuilding and installing box86...\n"
    cd ${HOME}/Programs
    git clone https://github.com/ptitSeb/box86.git
    cd box86
    mkdir build; cd build; cmake .. -D RPI4ARM64=1 -D CMAKE_BUILD_TYPE=RelWithDebInfo
    make -j4; sudo make install
    sudo systemctl restart systemd-binfmt 

    printf "\nInstalling steam...\n"
    cd ${HOME}/Programs/box86
    ./install_steam.sh

    printf "\nSteam is now hopefully installed. The computer will reboot at the end of the script. After it reboots, open a terminal and type \"steam\" then press ENTER to launch steam.\nPress ENTER to continue.\n"
    read pressEnter
fi

printf "\nWould you like to install SDL2? SDL2 is a C library for programming graphics and other media (Y/n): "
read install_sdl2
printf "\n"
if [ "${install_sdl2}" != "n" ]; then
    printf "\nInstalling SDL2...\n"
    sudo apt install jq pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev fcitx-libs-dev -y

    cd ${HOME}/Programs
    version=$(curl -sL https://api.github.com/repos/libsdl-org/SDL/releases/latest | jq -r '.name')
    wget $(curl -sL https://api.github.com/repos/libsdl-org/SDL/releases/latest | jq -r ".assets[].browser_download_url" | grep -m 1 "SDL2-${version}.tar.gz")
    tar -xf "SDL2-${version}.tar.gz"
    cd "SDL2-${version}"
    mkdir build
    cd build
    ../configure
    make
    sudo make install

    cd ${HOME}/Programs
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

printf "\nWould you like to install RetroPie? (Y/n): "
read install_retropi
printf "\n"
if [ "${install_retropi}" != "n" ]; then
    printf "\nInstalling RetroPi...\n"
    sudo apt install git lsb-release -y
    cd ${HOME}/Programs
    git clone --depth=1 https://github.com/RetroPie/RetroPie-Setup.git
    cd RetroPie-Setup
    chmod +x retropie_setup.sh
    sudo ./retropie_setup.sh
fi

printf "\nWould you like to install pigpio (https://github.com/joan2937/pigpio)? It is a C library for programming the computer's GPIO pins. (Y/n): "
read install_pigpio
printf "\n"
if [ "${install_pigpio}" != "n" ]; then
    printf "\nInstalling pigpio...\n"
    cd ${HOME}/Programs
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    rm -rf master.zip
    cd pigpio-master
    make
    sudo make install
fi

printf "\nEnsuring everything is up to date...\n"
sudo apt update
sudo apt full-upgrade -y

printf "\nCleaning up...\n"
sudo apt autoremove -y

printf "\nThe script has finished. Would you like to reboot now? <Y/n>: "
read reboot_now
printf "\n"
if [ ${reboot_now} == "n" ];
then
    printf "Rebooting... Have a nice day :)\n"
    sudo reboot
fi

printf "Have a nice day :)\n"

sudo reboot
