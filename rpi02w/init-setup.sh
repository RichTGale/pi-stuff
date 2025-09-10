#1/bin/bash

device_path="|${device_path}|"
device_path=s|${device_path}|
s device_path=|${device_path}|
#repo_name=${PWD##*/}
#repo_name=${repo_name:-/}
device_path=${PWD}


printf "\nWelcome to your rpi setup wizard! "
sleep 2
printf "During installation the rpi will restart a few times.\n"
sleep 5
printf "Please answer the fiollowing questions:\n"
sleep 2

printf "\nWould you like to install vim? <Y/n>: "
read install_vim
printf "\n"

printf "Would you like to install zsh? <Y/n>: "
read install_zsh
printf "\n"

printf "Would you like to modify the amount of swapfile space? <Y/n>: "
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
    sudo apt install vim -y
	curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	cp -t ${HOME} ${device_path}/.vimrc
	vim -c PlugInstall
fi

if [ "${install_zsh}" != "n" ]; then
	printf "\nInstalling zsh...\n"
    sudo apt install zsh -y
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

printf "Installing zerosec...\n"

printf "\nInstalling motion...\n"
sudo apt install motion -y
sudo cp ${device_path}/motion.conf /etc/motion/

printf "\nInstalling pigpio...\n"
cd ${HOME}/Programs
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
rm -rf master.zip
cd pigpio-master
make
sudo make install

printf "\nInstalling TP-Link Archer T2U Plus [RTL8821AU] driver...\n"
sudo apt install raspberrypi-kernel-headers build-essential git -y
cd ${HOME}/Programs
git clone https://github.com/lwfinger/rtw88
cd rtw88
sudo dkms install $PWD
sudo make install_fw

printf "\nBuilding IR light program...\n"
cp ${device_path}/let-there-be-light.c ${HOME}/Programs
cd ${HOME}/Programs
gcc -Wall -pthread -o let-there-be-light let-there-be-light.c -lpigpio -lrt

printf "\nCopying remote sending file(s)...\n"
cp ${device_path}/send-files.sh ${HOME}/Programs/
chmod +x $HOME/Programs/send-files.sh

printf "\nCopying autostart files...\n"
sudo cp -t /etc/xdg/autostart ${device_path}/autoapps.sh
cp -t ${HOME}/Programs ${device_path}/autoapps.sh
chmod +x ${HOME}/Programs/*.sh

printf "\nCopying config files...\n"
sudo cp ${device_path}/config.txt /boot/firmware/

sudo apt update
sudo apt upgrade -y
sudo apt autoremove -y

sudo raspi-config

sudo reboot
