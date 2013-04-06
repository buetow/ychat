#!/bin/sh

mke2fs /dev/ram0
mke2fs /dev/ram1

mkdir data 
cd data 
mkdir {logs,msgs,user,online}
mount /dev/ram0 msgs
mount /dev/ram1 online
cd user
mkdir {counts,memos,options,profiles,gbooks,lastlogin}
cd ../online
mkdir {rstat,rooms,pids,users,ident,tmpid,awake}
cd ..

chmod 700 * -R
chown yC * -R
