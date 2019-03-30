#######################################################################
# file name: AddSyntaxHi.sh
# author   : Yu Liu
# email    : <ilhanwnz@hotmail.com>
# time     : Fri 22 Feb 2019 06:08:53 PM CST
#######################################################################


VimPrefix="/usr/share/vim/vim80/syntax/"
FunctionNumbers=2

DisplaySelection() {
	echo "Which items do you want to select? (1-$FunctionNumbers)"
	echo "1. Add keyword highlight in programming language."
	echo "2. Create a new file in \"vim\" configure file."
	echo "3. Exit this script."
	echo -n "Your select (1-$FunctionNumbers): "
	SelectionItems;
}

SelectionItems() {
	read whichSelect;
	case "$whichSelect" in
		1) AddKeyWordInVimConf;;
		2) CreateNewFileInVimConf;;
		*) exit 1;;
	esac
}

AddKeyWordInVimConf() {
	echo -n "Please select file in vim: "
	read whichFile
	sudo vim $VimPrefix$whichFile.vim;
}

CreateNewFileInVimConf() {
	echo -n "Please enter a file name which you want to create: "
	read whichFile
	sudo echo "" > $VimPrefix$whichFile.vim;

}

DisplaySelection;
