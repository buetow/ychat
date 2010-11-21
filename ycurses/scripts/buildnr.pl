#!/usr/bin/perl

# The ycurses Project (2003)
#
# This script increases the BUILNR of src/msgs.h each time the ycurses
# gets recompiled!

use strict;

open MSGS, "src/msgs.h";
my @msgs = <MSGS>;
close MSGS;

foreach (@msgs)
{
 if ( /define BUILDNR/ )
 {
  s/(BUILDNR )(.+)$/$1.($2+1)/e;
  print;
  next;
 }
}

open MSGS, ">src/msgs.h";
print MSGS @msgs;
close MSGS;
