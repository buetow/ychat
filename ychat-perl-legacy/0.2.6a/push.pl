#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
CGI->compile(':all');
$q = new CGI;

$| = 1; # Cache von Perl ausschalten

print
 $q->header;	 # HTML-Header erzeugen

require config;

&secure_checkip($alias);

########################################
##ANFANG DER ZU STREAMENDEN HTML-DATEI##
########################################

print <<END;
<html>
 <head>
<script language="JavaScript">
<!--
// Scroll-Routine von Anklepper (http://www.anklepper.de)
// speziell für yChat (http://www.yChat.de), Copyright 2000
scrolling      = true;
moves_steps    = 4;  // Schrittgröße
moves_interval = 4;  // Schrittgeschwindigkeit

function moves()
{
  if( scrolling != false )
    window.scrollBy( 0, moves_steps );
  window.setTimeout( "moves()", moves_interval );
}

moves();
//-->
</script>
 </head>
<body bgcolor=000000 link=FFFFFF alink=FFFFEF vlink=FFFFEF>
END

#############################
##BEGRÜSSUNGSTEXT SCHREIBEN##
#############################

my $msgfile;

 print
   $q->font( { -size=>1,
               -color=>"ffffff",
               -face=>"arial"},
   	       "$title - Created & Copyright by Paul C. Bütow $tmp\n");

if ($var eq "push_logs_html") {
 $msgfile = $tmp;
 print   "<hr><font size=2 color=ffffff face=arial>";
} elsif ($var eq "clear_screen") {
 if (-f "data/user/memos/$alias") {  # Nach Memonachrichten checken!
  print "\n<br>";
  &read_file("data/user/memos/$alias");
   my $memos = <DATEI>;
  close DATEI;
  print $memos;
  unlink("data/user/memos/$alias");
 }
 $msgfile = "data/msgs/$alias";
 &write_file_new($msgfile);
  print DATEI $q->br;
 close DATEI;
} else {
 print $q->br;
 $msgfile = "data/msgs/$alias";
}

open (MSGFILE, $msgfile);

###############################
##ENDLOSSCHLEIFE FÜR DEN PUSH##
###############################

my ($times,$online) = (0,"false");
for (;;) {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
  print $_ ;
 }
 $times++;
 if ($times > 14) {			# Nach 15 Sekunden
  print "\n<!--PING//-->";		# Ping zum Browser senden
  $times = 0;				# Sekundenzahl auf 0 zurücksetzen
  $online = "true" if (-e "data/online/users/$alias");
  if ($online eq "false") {
    print "<font size=2 color=red>Aus technischen Gründen bitte auf <a href=\"push.pl?alias=$alias&room=$room&var=clear_screen\">Weiterchatten</a> klicken.</font>";
    exit;
  }
  $online = "false"
 }
 sleep 1;
 seek(MSGFILE, $curpos, 0); 		# Position wiederherstellen
}

