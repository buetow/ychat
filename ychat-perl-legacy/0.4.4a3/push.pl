#!/usr/bin/perl -w

# yChat - Copyright by Paul C. B�tow

use CGI;
CGI->compile(':all');
$q = new CGI;
print
 $q->header;
require config;

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
&secure_checkid($alias);

########################################
##ANFANG DER ZU STREAMENDEN HTML-DATEI##
########################################

print <<END;
<html>
 <head>
<script language="JavaScript">
<!--
// Scroll-Routine von Anklepper (http://www.anklepper.de)
// speziell f�r yChat (http://www.yChat.de), Copyright 2000
scrolling      = true;
moves_steps    = 4;  // Schrittgr��e
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
##BEGR�SSUNGSTEXT SCHREIBEN##
#############################

my $msgfile;

print
   $q->font( { -size=>1,
               -color=>"ffffff",
               -face=>"arial"},
   	       "$title - Created & Copyright by Paul C. B�tow $tmp\n");

if (-f "data/user/memos/$alias") {
 print "\n<br>";
 open(MEMOS,"<data/user/memos/$alias");
  my $memos = <MEMOS>;
 close MEMOS;
 print $memos;
 unlink("data/user/memos/$alias");
}

my $msgfile = "data/msgs/$alias";
open(MSG,">$msgfile");
 print MSG $q->br;
close MSG;
print $q->br;

###############################
##ENDLOSSCHLEIFE F�R DEN PUSH##
###############################

my ($times,$online) = (0,"false");
open(MSGFILE,$msgfile);
for (;;) {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
  print $_ ;
 }
 $times++;
 if ($times > 14) {
  print "\n<!--PING//-->";
  $times = 0;
  $online = "true" if (-e "data/online/users/$alias");
  if ($online eq "false") {
    print "<font size=2 color=red>Aus technischen Gr�nden bitte auf <a href=\"push.pl?alias=$alias&room=$room&var=clear_screen&tmpid=$tmpid\">Weiterchatten</a> klicken.</font>";
    exit;
  }
  $online = "false"
 }
 sleep 1;
 seek(MSGFILE, $curpos, 0); 		# Position wiederherstellen
}

