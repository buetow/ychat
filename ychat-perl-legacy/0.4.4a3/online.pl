#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print
 $q->header();

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

&start_html("Online","online");
&online($alias,$room);
opendir(ROOMS,"data/online/rooms/".$room);
 my @rooms = readdir(ROOMS);
closedir(ROOMS);
print
 $q->div( "$room:" ),
 "<font size=1 color=ffffef face=arial>";
  foreach $online (@rooms) {
  if (-f "data/online/rooms/$room/$online") {
   print
      $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$online&tmpid=$tmpid",
               -target=>"$online" }, "$online"), $q->br;
  }
 }
print
 "</font>",
 $q->end_html;

sub online { # Benutzer in die Onlineliste hinzufügen bzw. erneuern
 my ($alias,$room) = @_;
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
}