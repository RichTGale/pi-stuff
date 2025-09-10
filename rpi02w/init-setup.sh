#1/bin/bash

#repo_name=${PWD##*/}
#repo_name=${repo_name:-/}
device_path=${PWD}/rpi02w


printf "\nWelcome to your rpi setup wizard! "
sleep 2
printf "During installation the rpi will restart se eral times.\n"
sleep 2
printf "\nPlease answer the fiollowing questions:\n"
sleep 2

printf "\nWould you like to install vim? <Y/n>: "
read install_vim
printf "\n"

printf "\nWould you like to install zsh? <Y/n>: "
read install_zsh
printf "\n"

printf "\nWould you like to modify the amount of swapfile space? <Y/n>: "
read modify_swap
printf "\n"

printf "\nCreating ${HOME}/Programming and ${HOME}/Programs directories to install stuff in later...\n"
if [ ! -d ${HOME}/Programming ];
then
    mkdir ${HOME}/Programming
fi

if [ ! -d ${HOME}/Programs ];
then
    mkdir ${HOME}/Programs
	mkdir ${HOME}/Programs/videos
fi

printf "\nDoing a full system update and upgrade...\n"
sudo apt update
sudo apt full-upgrade -y

printf "\nInstalling some utilities and dependencies...\n"
sudo apt install git dkms build-essential cmake unzip openvpn libelf-dev linux-headers-$(uname -r) -y

if [ "${install_vim}" != "n" ]; then
	printf "\nInstalling vim...\n"
    sudo apt install vim
	curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	cp -t ${HOME} ${device_path}/.vimrc
	vim -c PlugInstall
fi

if [ "${install_zsh}" != "n" ]; then
	printf "\nInstalling zsh...\n"
    sudo apt install zsh
	sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
	git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
	git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
	cp -t ${HOME} ${device_path}/.zshrc
	printf "\nSwitching default shell to zsh...\n"
	sudo chsh -s $(which zsh)
fi

printf "\nCleaning up...\n"
sudo apt autoremove -y

if [ "${modify_swap}" != "n" ];
then
    printf "\nOpening the swap config file for editing...\n"
    sudo dphys-swapfile swapoff
    sudo vim /etc/dphys-swapfile
    sudo dphys-swapfile setup
    sudo dphys-swapfile swapon
fi

# Giving the other files the device_path
sed -i "${device_path}" ${device_path}/install-zerosec.sh
sed -i "${device_path}" ${device_path}/finalise-setup.sh

printf "\nCopying autostart files...\n"
sudo cp -t /etc/xdg/autostart ${device_path}/install-zerosec.desktop
cp -t ${HOME}/Programs ${device_path}/install-zerosec.sh 
cp -t ${HOME}/Programs ${device_path}/finalise-setup.sh 
cp -t ${HOME}/Programs ${device_path}/autoapps.sh
chmod +x ${HOME}/Programs/*.sh

sudo raspi-config

sudo reboot
