#!/bin/sh
# The yhttpd Project (2005)
#
# This scripts checks for a perl installation 

if ! which perl >/dev/null
then
 echo You need to have Perl in your PATH!
 exit 1
fi

echo Using `which perl`
exit 0
