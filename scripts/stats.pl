#!/usr/bin/perl

# The yChat Project (2003 - 2004)
#
# This script generates source code and project statistics 

use strict;

use scripts::modules::file;

my %stats;
my $param = shift;

&recursive(".");

$stats{"Lines total"} = $stats{"Lines of source"} 
                   + $stats{"Lines of scripts"}   
                   + $stats{"Lines of text"}   
                   + $stats{"Lines of HTML"};

unless (defined $param) {

 print "$_ = " . $stats{$_} . "\n"
  for ( sort keys %stats );

} else {

 print $stats{$_} . " "
  for sort keys %stats;

}

print "\n";

sub recursive
{
 my $shift = shift;
 my @dir = &dopen($shift);
 
 foreach (@dir)
 {
  next if /^\.$/ or /^\.{2}$/;

  if ( -f "$shift/$_" )
  {
   $stats{"Number of files total"}++;
   &filestats("$shift/$_");
  }
  elsif ( -d "$shift/$_" )
  {
   $stats{"Number of dirs total"}++;
   &recursive("$shift/$_");
  }
 }
}

sub filestats
{
 my $shift = shift;
 if ( $shift =~ /\.(cpp|h|tmpl)$/ )
 {
  $stats{"Number of source files"}++;
  $stats{"Lines of source"} += countlines($shift);
 }
 elsif ( $shift =~ /\.(html|css)$/ )
 {
  $stats{"Number of HTML files"}++;
  $stats{"Lines of HTML"} += countlines($shift);
 }
 elsif ( $shift =~ /\.(gif|png|jpg)$/ )
 {
  $stats{"Number of gfx files"}++;
 }
 elsif ( $shift =~ /(\.pl|\.sh|configure.*|Makefile.*)$/ )
 {
  $stats{"Number of script files"}++;
  $stats{"Lines of scripts"} += countlines($shift);
 }
 elsif ( $shift =~ /(\.txt|README|INSTALL|COPYING|NEWS|SNAPSHOT|ChangeLog)$/ )
 {
  $stats{"Number of text files"}++;
  $stats{"Lines of text"} += countlines($shift);
 }
 elsif ( $shift =~ /\.so$/ )
 {
  $stats{"Number of compiled module files"}++;
 }
}

sub countlines
{
 my $shift = shift;
 my @file = fopen($shift);
 return $#file; 
}
