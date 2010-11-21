#!/bin/sh
# The yhttpd Project (2005)
#
# This script shows yhttpd VERSION-BRANCH Build BUILDNUMBER 

version=`grep VERSION src/msgs.h | head -n 1 | cut -d'"' -f2`
branch=`grep BRANCH src/msgs.h | head -n 1 | cut -d'"' -f2`
build=`grep BUILD src/msgs.h | tail -n 1 | cut -d' ' -f3`
echo "yhttpd $version-$branch Build $build"


