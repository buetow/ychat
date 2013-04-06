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
my $pid = $$;
$| = 1;

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
<font size=1 color=ffffff face=arial>$title - Created & Copyright by Paul C. Bütow [PID: $pid]</font><br>
END

my $msgfile;


if (-f "data/user/memos/$alias") {
 open(MEMOS,"<data/user/memos/$alias");
  my $memos = <MEMOS>;
 close MEMOS;
 print $memos;
 unlink("data/user/memos/$alias");
}

my $startime,$change_room,@pids;
$SIG{INT} = \&sendmsgs;
OPENMSGFILE:
$change_room = 0;
$startime = time;

if (-e "data/msgs/$room") {
 open(MSGFILE,"<data/msgs/$room");
 open(PID,">>data/online/pids/$room");
  print PID $pid."\n";
 close PID;
} else {
 sleep 1;
 goto OPENMSGFILE;
}

&sendmsgs;

while (-e "data/online/users/$alias") {
 goto OPENMSGFILE if ($change_room == 1);
 sleep 15;
 print "<!--PING//-->\n";
}

sub sendmsgs {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
  my ($address, $stamp, $command, $action, $message) = split(/<;/, $_);
  if ($address eq "!" || $address eq $alias)  {
   if ($stamp > $startime) {
    if ($command eq "cr") {
     &closepid;
     $room = $action;
     close MSGFILE;
     $change_room = 1;
    }
    print "$message<br>\n" if ($change_room != 1);
   }
  }
 }
 seek(MSGFILE, $curpos, 0);
}

sub closepid {
 @pids = undef;
 open(PID,"<data/online/pids/$room");
  @pids = <PID>;
 close PID;
 my @newpids = undef;
 foreach(@pids) {
  push @newpids,$_ if ($pid ne $_."\n");
 }
open(PID,">data/online/pids/$room");
 print PID @newpids;
 close PID;
}

&closepid;