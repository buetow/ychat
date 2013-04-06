#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
CGI->compile(':all');
$q = new CGI;
print
 $q->header;
require config;

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
&secure_checkid($alias);
$room = $q->param("room");

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
   	       "$title - Created & Copyright by Paul C. Bütow [PID: $$]<br>\n");

if (-f "data/user/memos/$alias") {
 open(MEMOS,"<data/user/memos/$alias");
  my $memos = <MEMOS>;
 close MEMOS;
 print $memos;
 unlink("data/user/memos/$alias");
}

###############################
##ENDLOSSCHLEIFE FÜR DEN PUSH##
###############################

my $counter = 0;
my $startime;
OPENMSGFILE:
$startime = time;

if (-e "data/online/users/$alias") {
 open(MSGFILE,"<data/msgs/$room");
} else {
 sleep 1;
 goto OPENMSGFILE;
}

while (-e "data/online/users/$alias") {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
 my ($address, $stamp, $command, $action, $message) = split(/<;/, $_);
 if ($address eq "!" || $address eq $alias)  {
  if ($stamp > $startime) {
   if ($command eq "cr") {
    $room = $action;
    close MSGFILE;
    goto OPENMSGFILE;
   }
   print "$message<br>\n";
  }
 }
 }
 $counter++;
 if ($counter > 14) {
  print "<!--PING//-->\n";
  $counter = 0;
 }
 sleep 1;
 seek(MSGFILE, $curpos, 0);
}