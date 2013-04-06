#!/bin/sh

# The yChat Project (2004, 2005)
#
# This uses "astyle" to format C++ code into a specific code style! 

for f in h cpp tmpl
do
 for i in `find . -name "*.$f"`
 do
  echo $i
  astyle --style=ansi -s2 $i
  rm -f $i.orig
 done
done
