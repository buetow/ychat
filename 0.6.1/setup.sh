#!/bin/sh
echo
echo yChat wird konfiguriert!
echo
mkdir data
chmod 777 data
mkdir data/logs
chmod 777 data/logs
mkdir data/msgs
chmod 777 data/msgs
mkdir data/user
chmod 777 data/user
mkdir data/user/counts
chmod 777 data/user/counts
mkdir data/user/memos
chmod 777 data/user/memos
mkdir data/user/options
chmod 777 data/user/options
mkdir data/user/profiles
chmod 777 data/user/profiles
mkdir data/user/gbooks
chmod 777 data/user/gbooks
mkdir data/user/lastlogin
chmod 777 data/user/lastlogin
mkdir data/online
chmod 777 data/online
mkdir data/online/rooms
chmod 777 data/online/rooms
mkdir data/online/pids
chmod 777 data/online/pids
mkdir data/online/rstat
chmod 777 data/online/rstat
mkdir data/online/users
chmod 777 data/online/users
mkdir data/online/ident
chmod 777 data/online/ident
mkdir data/online/tmpid
chmod 777 data/online/tmpid
chmod 777 data/htdocs/*
chmod 777 *.pl
chmod 777 config.pm
mkdir dokus/
mv *.txt dokus/
mkdir ../../htdocs/yChat/
mv *.gif ../../htdocs/yChat/
mv *.swf ../../htdocs/yChat/

echo
echo yChat Setup beendet ...
echo Created and Copyright by Paul C. Bütow
echo
