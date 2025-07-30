#1/bin/bash


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

printf "\nWould you like to configure vim? (Y/n):"
read install_vim
printf "\n"
if [ "$install_vim" != "n" ]; then
	printf "\nConfiguring vim...\n"
	cd $HOME/Programming
	curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	cp $HOME/Programming/pi-stuff/.vimrc $HOME/
	vim -c PlugInstall
fi

printf "\nWould you like to configure zsh? (Y/n):"
read install_zsh
printf "\n"
if [ "$install_zsh" != "n" ]; then
	printf "\nConfiguring zsh...\n"
	sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
	git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
	git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
	cd $HOME/Programming
	cp $HOME/Programming/pi-stuff/.zshrc $HOME/
	printf "\nSwitching default shell to zsh...\n"
	chsh -s $(which zsh)
fi

printf "\nCleaning up...\n"
sudo apt autoremove -y

printf "\nCopying autostart files...\n"
cp $HOME/Programming/pi-stuff/configure-pi02w-01.sh $HOME/Programming/pi-stuff/configure-pi02w-02.sh $HOME/Programming/pi-stuff/configure-pi02w-03.sh $HOME/Programming/pi-stuff/configure-pi02w-04.sh HOME/Programming/pi-stuff/configure-pi02w-05.sh -t $HOME/Programs
chmod +x $HOME/Programs/configure-pi02w-01.sh
chmod +x $HOME/Programs/configure-pi02w-02.sh
chmod +x $HOME/Programs/configure-pi02w-03.sh
chmod +x $HOME/Programs/configure-pi02w-04.sh
chmod +x $HOME/Programs/configure-pi02w-05.sh
sudo cp $HOME/Programming/pi-stuff/configure-pi02w-01.desktop /etc/xdg/autostart/

sudo reboot
