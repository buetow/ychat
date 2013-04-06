#!/usr/bin/perl -w

# yChat - Copyright by Paul C. B�tow

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
$room = $q->param("room");

&secure_checkid($alias);

# Onlinezeit z�hlen !
&read_file("data/online/users/$alias");
 my $oldstamp = <DATEI>;
close DATEI;
 if ($oldstamp > (time - 60)) {
  &read_file("data/user/counts/$alias.online");
   my $online = <DATEI>;
  close DATEI;
  my $total = (time-$oldstamp+$online);
  &write_file_new("data/user/counts/$alias.online");
   print DATEI $total;
  close DATEI;
 }
##### ENDE

&start_html("Online","online");
&online($alias,$room);
&list_dir("data/online/rooms/".$room);
print
 $q->div( "$room:" ),
 "<font size=1 color=ffffef face=arial>";
  foreach $online (@dir) {
  if (-f "data/online/rooms/$room/$online") {
   print
      $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$online&tmpid=$tmpid",
               -target=>"$online" }, "$online"), $q->br;
  }
 }
print
 "</font>",
 $q->end_html;

sub online { # Benutzer in die Onlineliste hinzuf�gen bzw. erneuern
 my ($alias,$room) = @_;
 &write_file_new("data/online/users/$alias");
  print DATEI time;
 close DATEI;
 if (-f "data/online/rooms/$room") { } else { mkdir "data/online/rooms/$room", 0777; }
 if (-f "data/online/rstat/$room") { } else {
  &write_file_new("data/online/rstat/$room"); # Raumbesetzerstatus setzen
   print DATEI "0\n\n$alias\n";
  close DATEI;
 }
 &write_file_new("data/online/rooms/$room/$alias");
  print DATEI time;
 close DATEI;
 &read_file("data/online/prove");       # R�ume und Chatter auf Existenz �berpr�fen
  my $provetime = <DATEI>;
 close DATEI;
 if ($provetime < (time - 30)) {
  &check_online;
 }
}