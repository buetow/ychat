#!/usr/bin/perl

# The yChat Project (2003)
#
# This script sets up some variables in src/glob.h 

use strict;

use scripts::modules::file;

my $file = 'src/glob.h';
my $gmake = `which gmake`;
my @glob = fopen($file);
chomp($gmake);

print "-> Setting values in $file\n";

my $modified = 0;
foreach (@glob)
{
 if (/^(#define GMAKE) "(.*)"/)
 {
  if ($2 ne "$gmake \\0")
  {
   s/^$1 "$2"/#define GMAKE "$gmake \\0"/;
   print " -> Set $gmake\n";
   fwrite($file,@glob);  
   last;  
  }
 }
}

