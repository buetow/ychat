#!/bin/sh

rm *.so 2> /dev/null

for i in *cpp
do
 echo Compiling $i
 gcc -shared -o $(echo $i | sed s/cpp$/so/) $i
done
