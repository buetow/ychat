#!/usr/bin/perl

# The yChat & yhttpd Project (2004, 2005)
#
# This scripts modifies the yChat sources to yhttpd sources.

use strict;
use scripts::modules::file;

my @delete = ( 
 'CHANGES',
 'g++.version',
 'TODO',
 'NEWS',
 'docs',
 'src/chat',
 'src/memb',
 'src/data',
 'src/irc',
 'src/contrib/crypt',
 'src/modl.h',
 'src/modl.cpp',
 'src/mods',
 'obj',
 'mods',
 'html',
 'log/rooms',
 'scripts/makeyhttpd.pl',
 'scripts/mergeconf.pl',
 'bin/ychat',
 'bin/ychat.old',
);	

my @createdir = (
 'html'
);

my %substituate = (
  'ychat' => 'yhttpd',
  'yChat' => 'yhttpd',
  'YCHAT' => 'YHTTPD',
  'CHAT' => 'HTTPD',
  'yhttpd.org' => 'yChat.org',
  '//>>' => ''
);

if ( -d "../yhttpd" ) {
  print "Removing ../yhttpd\n";
  system("rm -Rf ../yhttpd");
}

print "Creating new ../yhttpd\n";
system("cp -Rp ../ychat ../yhttpd");

chdir("../yhttpd");
system("gmake clean");

my @deletelines;
foreach (@delete) {
  next unless -d $_;
  foreach (dopen($_)) {
    next unless /\.h$/;
    push @deletelines, $_;
  }
}

print "Moving html templates\n";
system("mv html/test.cgi .");
system("mv html/demo.html .");
system("mv html/style.css .");
system("mv html/notfound.html .");

print "Removing dirs and files\n->";
foreach (@delete) {
  next unless -f $_ || -d $_;
  print " $_";
  system("rm -Rf $_"); 
}

print "\nDeleting CVS directories\n";
system("find . -name CVS | xargs rm -Rf");

print "Creating new dirs\n->";
foreach (@createdir) {
  print " $_";
  system("mkdir $_"); 
}

print "\nRenaming config file\n";
system("mv etc/ychat.conf etc/yhttpd.conf");

print "Moving html templates\n";
system("mv demo.html html/index.html");
system("mv test.cgi notfound.html style.css html");

print "Editing etc/yhttpd.conf\n";
&edit_yhttpd_conf();
print "Removing marked lines of code\n->";
&remove_marked_lines('.');
print "\nEdit version numbers\n->";
&edit_version_numbers('yhttpd/src/msgs.h','yhttpd/README');

sub remove_marked_lines {
 my $dir = shift;
 chdir($dir);

 for (&dopen("."))
 {
   next if /^\.+$/;
   print " $_";

   if ( -f $_ )
   {
     my @newfile;
     my $flag = 0;

     for my $line (fopen($_))
     {
	$flag = 1 if $line =~ /\/\/<<\*/;	
        if ($flag == 0 && $line !~ /\/\/<</) {
          for ( @deletelines )
          {
             if ($line =~ /$_/)
             {
                $flag = 3;
                last;
             }
          }

          if ($flag != 3 )
          {
            map { $line =~ s/$_/$substituate{$_}/eg } keys %substituate;
            push @newfile, $line;
          } 

          else
          {
            $flag = 0;
          }
        }
	$flag = 0 if $line =~ /\/\/\*>>/;	
     }

     &fwrite($_, @newfile);
   } 

   elsif ( -d $_ )
   {
     # Recursive
     &remove_marked_lines($_);
   }
 }

 chdir('..');
}

sub edit_yhttpd_conf
{
  my @old = fopen("etc/yhttpd.conf");
  my @new = @old[0..1];

  my $flag = 0;
  for (@old)
  {
   if ($flag == 0)
   { 
     if (/<category name="httpd">/)
     {
       $flag = 1;
       push @new, $_;	
     }
   }

   else
   {
     push @new, $_;
   }
  }

  fwrite("etc/yhttpd.conf", @new); 
}

sub edit_version_numbers
{
  for (@_)
  {
    print " $_";
    my @file = fopen $_;
    for (@file)
    {
      s/([0-9]+\.[0-9]+)\.[0-9]+(-*)/$1$2/g for @file;
      s/RELEASE/DEVEL/g;
    }
    fwrite($_,@file);
  }
}

print "\n";
