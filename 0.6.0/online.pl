#!/usr/bin/perl

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print $q->header();

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
&secure_checkid($alias);
$room = $q->param("room");

open(DATEI,"<data/online/rstat/$room.away");
 my @datei = <DATEI>;
close DATEI;
foreach(@datei) {
 push @away,split(/<; /);
}

#my %away = &prove_away($alias,$room,"return_list");

#print "<font color=red>das ist ein test: ";
#for ($i=0;$i<=$#away;$i+=2) {
#   print "$away[$i]<br>";
#  }
#print "</font>";

for ($i=0;$i<=$#away;$i+=2) {
 goto DONOTCOUNT if ($away[$i] eq $alias);
}

open(DATEI,"<data/online/users/$alias");
 my $oldstamp = <DATEI>;
close DATEI;
if ($oldstamp > (time - 40)) {
 open(DATEI,"<data/user/counts/$alias.online");
  my $online = <DATEI>;
 close DATEI;
 my $total  = (time-$oldstamp+$online);
 open DATEI,">data/user/counts/$alias.online";
  print DATEI $total;
 close DATEI;
}
DONOTCOUNT:

unless (-d "data/online/rooms/$room") {
 mkdir "data/online/rooms/$room", 0777;
}
unless (-f "data/online/rstat/$room") {
 open(DATEI,">data/online/rstat/$room"); # Raumbesetzerstatus setzen
  print DATEI "0\n\n$alias\n";
 close DATEI;
}

open(DATEI,">data/online/users/$alias");
 print DATEI time;
close DATEI;
open(DATEI,">data/online/rooms/$room/$alias");
 print DATEI time;
close DATEI;

open(DATEI,"<data/online/prove");
 my $provetime = <DATEI>;
close DATEI;
&check_online if ($provetime < (time - 30));

print<<HEADER;
<html>
 <head>
  <meta http-equiv="refresh" content="20" URL="online.pl?room=$room&alias=$alias&tmpid=$tmpid">
  <style type="text/css">
   a { font-family: arial; font-size:11px; color: #ffffef; }
   a:hover { color: #ffffff; }
  </style>
  <style type="text/css" media="all">
   a { text-decoration: none; }
   a:hover { text-decoration:underline; }
  </style>
 </head>
 <body bgcolor="#000000">
HEADER

opendir(ROOM,"data/online/rooms/".$room);
 my @room = sort(readdir(ROOM));
closedir(ROOM);

my $chatter = $#room-1;
my @besetzer = &prove_besetzer(undef,$room,"return_list");

open WA, "<data/wa";
 my @wa = <WA>;
close WA;
open OW, "<data/ow";
 my @ow = <OW>;
close OW;

print "<font size='1' color='ffffff' face='arial'>$room ($chatter):</font><table>";

foreach $online (@room) {
 if (-f "data/online/rooms/$room/$online") {
  for ($i=0;$i<=$#away;$i+=2) {
   chomp $away[$i+1];
   if ($away[$i] eq $online) {
    my $away = $away[$i+1];
    print "<tr><td><img title='$away' alt='$away' src='$gfxpath/away.gif'></td>";
    goto WEITER;
   }
  }
  foreach(@besetzer) {
   chomp;
   if ($_ eq $online) {
    print "<tr><td><img title='Raumbesetzer' alt='Raumbesetzer' src='$gfxpath/rb.gif'></td>";
    goto WEITER;
   }
  }
  foreach(@wa) {
   chomp;
   if ($_ eq $online) {
    print "<tr><td><img title='W&auml;chter' alt='W&auml;chter' src='$gfxpath/wa.gif'></td>";
    goto WEITER;
   }
  }
  foreach(@ow) {
   chomp;
   if ($_ eq $online) {
    print "<tr><td><img title='Oberw&auml;chter' alt='Oberw&auml;chter' src='$gfxpath/ow.gif'></td>";
    goto WEITER;
   }
  }
 print "<tr><td></td>";
WEITER:
  print "<td><a href='main2.pl?var=alias_html&alias=$alias&tmp=$online&tmpid=$tmpid' target='$online'>$online</a></td></tr>";
 }
}

print "</table>", $q->end_html;
