#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen

&secure_checkip($alias);
&start_html("Online","online");
&online($alias,$room);
&list_dir("data/online/rooms/".$room);
print
 $q->div( "$room:" ),
 "<font size=1 color=ffffef face=arial>";
  foreach $online (@dir) {
  if (-f "data/online/rooms/$room/$online") {
   print
      $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$online",
               -target=>"$online" }, "$online"), $q->br;
  }
 }
print
 "</font>",
 $q->end_html;

sub online { # Benutzer in die Onlineliste hinzufügen bzw. erneuern
 my ($alias,$room) = @_;
 &write_file_new("data/online/users/$alias");
  print DATEI time;
 close DATEI;
 if (-e "data/online/rooms/$room") {    # Falls Raum nicht existiert anlegen
 } else {
  &write_file_new("data/online/rstat/$room"); # Raumbesetzerstatus setzen
   print DATEI "0\n\n$alias\n";
  close DATEI;
  mkdir "data/online/rooms/$room", 0777;
 }
 &write_file_new("data/online/rooms/$room/$alias");
  print DATEI time;
 close DATEI;
 &read_file("data/online/prove");       # Räume und Chatter auf Existenz überprüfen
  my $provetime = <DATEI>;
 close DATEI;
 if ($provetime < (time - 30)) {
  &write_file_new("data/online/prove");
   print DATEI time;
  close DATEI;
  &rm_alias("data/online/users");
  &rm_rooms;
 }
}
