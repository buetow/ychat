#!/bin/sh
# The yChat Project (2005)
#
# This script shows yChat VERSION-BRANCH Build BUILDNUMBER 

version=`grep VERSION src/build.h | head -n 1 | cut -d'"' -f2`
branch=`grep BRANCH src/build.h | head -n 1 | cut -d'"' -f2`
build=`grep BUILD src/build.h | tail -n 1 | cut -d' ' -f3`
echo "yChat $version-$branch Build $build"


