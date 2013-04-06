#!/bin/sh
# The yChat Project (2005)
#
# This script creates a .tar.bz2 ball. 

pwd=`pwd` && base=`basename $pwd` && cd .. &&
version=`grep VERSION $base/src/msgs.h | head -n 1 | cut -d'"' -f2` &&
prefix="$base-$version"
package="$prefix.tar.bz2"

echo Copying files... &&
rm -Rf $prefix 2>/dev/null >/dev/null
cp -Rp $base $prefix && cd $prefix &&

echo Cleaning... &&
gmake mrproper 2>/dev/null >/dev/null
find . -name CVS | xargs rm -Rf
cd .. &&

echo Compressing... &&
if test -f $package; then rm -f $package; fi &&
tar cjf $package $prefix &&

echo Finishing... &&
rm -Rf $prefix 2>/dev/null >/dev/null 
cd $base &&
du -h ../$package
