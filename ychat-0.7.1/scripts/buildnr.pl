#!/usr/bin/perl

# The yChat Project (2003)
#
# This script increases the BUILNR of msgs,h each time the yChat
# gets recompiled!

use strict;

open MSGS, "src/msgs.h";
my @msgs = <MSGS>;
close MSGS;

foreach (@msgs)
{
 if ( /BUILDNR/ )
 {
  s/(BUILDNR )(.+)$/$1.($2+1)/e;
  print;
  next;
 }
}

open MSGS, ">src/msgs.h";
print MSGS @msgs;
close MSGS;
