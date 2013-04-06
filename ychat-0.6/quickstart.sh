#!/bin/sh
STARS="----------------------------------------\n"
echo "Welcome to yChat 0.5 Quickstart"
echo -e $STARS
echo "Copyright 2003, Paul C. Buetow - Volker Richter"
echo "yChat++ is developed under GNU Public License"
echo "http://www.ychat.org"
echo -e "\n"
echo -e "Press enter to begin quickstart"
read 
echo "STEP I"
echo -e "$STARS PREPARING SOURCES\n\n\n"
./configure 
if [ ! -e ./Makefile ]; then
	echo "STEP I UNSUCCESSFUL"
	exit 1;
else
	echo "STEP II COMPILING SOURCES"
fi
make &&
cd mods &&
./compile.sh &&
cd - 
if [ ! -x ./ychat ]; then
	echo "STEP II UNSUCCESSFUL"
	exit 1
fi
echo -e "\n\nSTEP II SUCCESSFUL\n\n"
echo -e "yChat is now ready to go. You can start yChat by running ./ychat from your console.\n\n"
echo -e "Should i start the yChat now? [N/y]: "
read startchat
if [ $startchat == "y" ]; then
	./ychat
fi
