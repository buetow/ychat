#!/usr/bin/perl

# The yhttpd Project (2004, 2005)
#
# This script modifues the src/glob.h file.

use strict;
use scripts::modules::file;

print <<END;
Welcome to the yhttpd configurator!
You may also edit the src/glob.h file manually instead of using 
this configurator option. Please also notice that this are only
before-compile options. All setups which can be made after com-
iling are placed in the yhttpd configuration file. 
END
 
my $sep = "================================================================\n"; 
my $stdin;

for (;;) {
 print "$sep Do you want to use the default before-compile options?\n";
 print " (yes/no) [default is NO] ";
 $stdin = <STDIN>;
 chomp $stdin;
 print "\n";

 if ( $stdin eq "yes") {
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

for (@glob) {
 if ( $flag == 0 && /- CONFIG -/ ) {
  print $sep;
  $flag = 1;
  next;

 } elsif ( $flag == 1 ) {
  if ( /\*\// ) {
   $flag = 2;

  } else {
   print;
  }  

  next;

 } elsif ( $flag == 2 ) {
  if ( /#define (.+) (.+)/ ) {
   my $def = $1;
   my $val = $2;
   my $flg = 0;

   $flg = 1 if $val =~ s/"//g;

   print " [Press enter to use default value: $val or q to quit] ";
   $stdin = <STDIN>;
   chomp $stdin;

   finish() if $stdin eq "q" or $stdin eq "quit";

   unless ( parse_input_value(\$stdin,\$val) ) {
    $stdin = "\"$stdin\"" if $flg == 1;
    $_ = "#define $def $stdin\n";
   }
   
   print "\n";
   $flag = 0;
   next;
  } 

  elsif ( /#define .+/ ) {
   my $default = "true";
   my $stdin;

   for (;;) {
    $default = "false" if /\/\/#define/;

    print " [Press enter to use default value: $default or q to quit] ";
    $stdin = <STDIN>;
    chomp $stdin; 

    finish() if $stdin eq "q" or $stdin eq "quit";
    last if $stdin eq "" || $stdin eq "true" || $stdin eq "false";
    print " Wrong input: You need to specify true or false!\n";
   }

   if ( $default eq "true" ) {
    $_ = "//$_" unless parse_input_value(\$stdin,\$default);
   }

   else {
    s/^\/\/// unless parse_input_value(\$stdin,\$default);
   }
  
   print "\n";
   $flag = 0;
   next;
  } 
 }
} // for

finish();

sub finish() {
  fwrite("src/glob.h", @glob);
  exit 0;
}
 
sub parse_input_value {
  my $val = shift;
  my $def = shift;

  if ( $$val eq "" || $$val eq $$def) {
   print " -> Using default option!\n";
   return 1;
  }

  print " -> Using new option $$val!\n";
  return 0;
}
