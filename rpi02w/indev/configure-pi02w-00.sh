#1/bin/bash

dir_path=${PWD}
dir_name=${${PWD##*/}}

printf "\nCreating ${HOME}/Programming and ${HOME}/Programs directories to install stuff in later...\n"
if [ ! -d ${HOME}/Programming ]; then
    mkdir ${HOME}/Programming
fi

if [ ! -d ${HOME}/Programs ]; then
    mkdir ${HOME}/Programs
fi

cp ${dir_path} -t ${HOME}/Programs
cd ${HOME}/Programs/${dir_name}

dir_path=${PWD}

printf "\nDoing a full system update and upgrade...\n"
sudo apt update
sudo apt full-upgrade -y

printf "\nInstalling some utilities and dependencies...\n"
sudo apt install vim git zsh dkms build-essential cmake unzip openvpn libelf-dev linux-headers-$(uname -r) -y

printf "\nWould you like to configure vim? (Y/n):"
read install_vim
printf "\n"
if [ "${install_vim}" != "n" ]; then
	printf "\nConfiguring vim...\n"
	curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	cp ${dir_path}/.vimrc -t ${HOME}
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
	cp ${dir_path}/.zshrc -t ${HOME}
	printf "\nSwitching default shell to zsh...\n"
	sudo chsh -s $(which zsh)
fi

printf "\nCleaning up...\n"
sudo apt autoremove -y

printf "\nCopying autostart files...\n"
chmod +x ${dir_path}/*.sh
sudo cp ${dir_path}/configure-pi02w-01.desktop -t /etc/xdg/autostart/

sudo raspi-config

sudo reboot
