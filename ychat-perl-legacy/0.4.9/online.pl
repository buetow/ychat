#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print $q->header();

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
$room = $q->param("room");

&secure_checkid($alias);

# Onlinezeit zählen !
open(DATEI,"<data/online/users/$alias");
 my $oldstamp = <DATEI>;
close DATEI;
 if ($oldstamp > (time - 60)) {
  open(DATEI,"<data/user/counts/$alias.online");
   my $online = <DATEI>;
  close DATEI;
  my $total = (time-$oldstamp+$online);
  open(DATEI,">data/user/counts/$alias.online");
   print DATEI $total;
  close DATEI;
 }
##### ENDE

open(DATEI,">data/online/users/$alias");
 print DATEI time;
close DATEI;
if (-f "data/online/rooms/$room") { } else { mkdir "data/online/rooms/$room", 0777; }
if (-f "data/online/rstat/$room") { } else {
 open(DATEI,">data/online/rstat/$room"); # Raumbesetzerstatus setzen
  print DATEI "0\n\n$alias\n";
 close DATEI;
}
open(DATEI,">data/online/rooms/$room/$alias");
 print DATEI time;
close DATEI;
open(DATEI,"<data/online/prove");       # Räume und Chatter auf Existenz überprüfen
 my $provetime = <DATEI>;
close DATEI;
if ($provetime < (time - 30)) {
 &check_online;
}

&start_html("Online","online");
opendir(ROOM,"data/online/rooms/".$room);
 my @room = readdir(ROOM);
closedir(ROOM);
my $chatter = $#room-1;
print $q->div( "$room ($chatter):" ), "<table>";
 @room = sort(@room);
 foreach $online (@room) {
  if (-f "data/online/rooms/$room/$online") {
   &prove_away($online,$room);
   if ($away ne undef) {
    print "<tr><td><img title='$away' alt='$away' src='$gfxpath/away.gif'></td>";
    goto WEITER;
   }
   &hierachie($online);
   if ($hierachie eq "ow") {
    print "<tr><td><img title='Oberw&auml;chter' alt='Oberw&auml;chter' src='$gfxpath/ow.gif'></td>";
    goto WEITER;
   } elsif ($hierachie eq "wa") {
    print "<tr><td><img title='W&auml;chter' alt='W&auml;chter' src='$gfxpath/wa.gif'></td>";
    goto WEITER;
   }
   &prove_besetzer($online,$room);
   if ($r_besetzer eq "true") {
    print "<tr><td><img title='Raumbesetzer' alt='Raumbesetzer' src='$gfxpath/rb.gif'></td>";
   } else {
    print "<tr><td></td>";
   }
WEITER:
   print "<td><a href='main2.pl?var=alias_html&alias=$alias&tmp=$online&tmpid=$tmpid' target='$online'><font size='1' face='arial'>$online</font></a></td></tr>";
  }
 }
print "</table>", $q->end_html;


