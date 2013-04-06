#!/bin/sh
# The yChat Project (2005)
#
# This script shows yChat VERSION-BRANCH 

sed -n 's/.*VERSION.*: \(.*\)".*/\1/p' src/msgs.h
