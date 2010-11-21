#!/bin/sh
# The ycurses Project (2005)
#
# This script shows ycurses VERSION-BRANCH Build BUILDNUMBER 

version=`grep VERSION src/msgs.h | head -n 1 | cut -d'"' -f2`
branch=`grep BRANCH src/msgs.h | head -n 1 | cut -d'"' -f2`
build=`grep BUILD src/msgs.h | tail -n 1 | cut -d' ' -f3`
echo "ycurses $version-$branch Build $build"


