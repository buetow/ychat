#!/bin/sh
mkdir data
mkdir data/logs
chmod 777 data/logs
mkdir data/msgs
chmod 777 data/msgs
mkdir data/logs/ow
chmod 777 data/logs/ow
mkdir data/user
chmod 777 data/user
mkdir data/user/counts
chmod 777 data/user/counts
mkdir data/user/options
chmod 777 data/user/options
mkdir data/user/profiles
chmod 777 data/user/profiles
mkdir data/user/gbooks
chmod 777 data/user/gbooks
mkdir data/online
chmod 777 data/online
mkdir data/online/rooms
chmod 777 data/online/rooms
mkdir data/online/users
chmod 777 data/online/users
mkdir data/online/ident
chmod 777 data/online/ident
mkdir data/htdocs
chmod 777 data/htdocs
mv help_html data/htdocs
mv logs_html data/htdocs
chmod 777 data/htdocs/*
chmod 777 *
mkdir dokus/
mv *.txt dokus/
mkdir ../../htdocs/yChat/
mv *.gif ../../htdocs/yChat/
mv *.jpg ../../htdocs/yChat/
echo
echo yChat Setup beendet ...
echo Created and copyrighted by Paul C. Bütow aka Snooper
echo
rm setup.sh
