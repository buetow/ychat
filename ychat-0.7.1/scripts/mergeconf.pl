#!/usr/bin/perl

# The yChat Project (2003)
#
# This script merges configuration files 

use strict;

use scripts::modules::file;

my @files = ('conf.txt','lang/en.txt','lang/de.txt');

foreach my $filename ( @files )
{
 next if ( ! -f $filename || ! -f "$filename.old" );

 print "=> Merge $filename\n";
 my @newoptions = &merge("$filename.old",$filename );
 if (defined $newoptions[0])
 {
  print " => Modified $filename. Edit to set the new values [or leave standard]\n  => New options are:"; 
  foreach (@newoptions)
  {
   print " $_";
  }
  print "\n";
 }
} 

sub merge
{
 my ($oldfile,$newfile) = @_;
 my @oldfile = fopen($oldfile); 
 my @newfile = fopen($newfile); 
 my %oldfile = get_options(@oldfile);
 my %newfile = get_options(@newfile);
 my @newoptions;

 foreach my $option (keys %newfile)
 {
  unless (defined $oldfile{$option})
  {
   push @newoptions, $option;
   my $_comment = $newfile{$option}[0]; 
   my $_value   = $newfile{$option}[1]; 
   my $_comment_b = $newfile{$option}[2]; 
   my $_behind  = $newfile{$option}[3]; 
   print " => Adding to $newfile\n  => New option is $option\n"
         ."  => Adding it after option $_behind\n";

   my $added = 0;
   my $insert = $_comment . "$option=\"$_value\";$_comment_b\n"; 
   foreach (@oldfile)
   {
    if (/^$_behind.+$/)
    {
     $_ .= $insert; 
     $added = 1;
     last;
    }
   }
   if ( $added == 0 )
   {
    print " => Adding new option at EOF\n";
    push @oldfile, $insert; 
   }
  }
 }

 &fwrite($newfile,@oldfile);
 
 return @newoptions;
}

sub get_options
{
 my @file = @_;
 my %options;
 my $option_comment;
 my $option_before = '';

 foreach (@file)
 {
  if (/(^.+)="(.+)";(.*)/)
  {
   my $option = $1;
   my $option_value = $2;
   my $option_comment_behind = $3;
   my @values = ($option_comment, 
                 $option_value,
                 $option_comment_behind,
                 $option_before);
   $options{$option} = \@values;
   $option_comment = '';
   $option_before = $option;
  }
  else
  {
   $option_comment .= $_;
  }
 }

 return %options;
}
