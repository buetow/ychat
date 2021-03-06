#!/usr/bin/perl

# The ycurses Project (2004, 2005)
#
# This script modifues the src/glob.h file.

use strict;
use scripts::modules::file;

print <<END;
Welcome to the ycurses configurator!
You may also edit the src/glob.h file manually instead of using 
this configurator option. Please also notice that this are only
before-compile options. All setups which can be made after com-
iling are placed in the ycurses configuration file. 
END
 
my $sep = "================================================================\n"; 
my $stdin;

for (;;)
{
 print "$sep Do you want to use the default before-compile options?\n";
 print " (yes/no) [default is NO] ";
 $stdin = <STDIN>;
 chomp $stdin;
 prove_if_default(\$stdin);
 print "\n";

 if ( $stdin eq "yes")
 {
   print " You chose to use all the default before-compile options. Exiti-\n";
   print " ng the configurator now!\n";    
   print $sep;
   exit(0);
 }

 last if $stdin eq "no" or $stdin eq "";
 print " Wrong input: You need to specify yes or no!\n";
} # for

`cp src/glob.h src/glob.h.org` unless -f "src/glob.h.org";

my @glob = fopen("src/glob.h.org");
my $flag = 0;

for (@glob) 
{
 if ( $flag == 0 && /- CONFIG -/ )
 {
  print $sep;
  $flag = 1;
  next;
 } 

 elsif ( $flag == 1 )
 {
  if ( /\*\// )
  {
   $flag = 2;
  }

  else 
  {
   print;
  }  

  next;
 }

 elsif ( $flag == 2 )
 {
  if ( /#define (.+) (.+)/ )
  {
   my $def = $1;
   my $val = $2;
   my $flg = 0;

   $flg = 1 if $val =~ s/"//g;

   print " [Press enter to use default value: $val] ";
   $stdin = <STDIN>;
   chomp $stdin;

   unless ( prove_if_default(\$stdin) )
   {
    $stdin = "\"$stdin\"" if $flg == 1;
    $_ = "#define $def $stdin\n";
   }
   
   print "\n";
   $flag = 0;
   next;
  } 

  elsif ( /#define .+/ ) 
  {
   my $default = "true";
   my $stdin;

   for (;;)
   {
    $default = "false" if /\/\/#define/;

    print " [Press enter to use default value: $default] ";
    $stdin = <STDIN>;
    chomp $stdin; 

    last if $stdin eq "" || $stdin eq "true" || $stdin eq "false";
    print " Wrong input: You need to specify true or false!\n";
   }

   if ( $default eq "true" )
   {
    $_ = "//$_"
     unless prove_if_default(\$stdin);
   }

   else
   {
    s/^\/\/// 
     unless prove_if_default(\$stdin);
   }
  
   print "\n";
   $flag = 0;
   next;
  } 
 }
} // for

 fwrite("src/glob.h", @glob);
 
sub prove_if_default
{
  my $val = shift;
  if ( $$val eq "" )
  {
   print " -> Using default option!\n";
   return 1;
  }
  print " -> Using new option $$val!\n";
  return 0;
}
