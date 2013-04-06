#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
#CGI->compile(':all');
$q = new CGI;
print $q->header;

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");

{
 open(IDENT,"<data/online/ident/$alias");
  my @ident = <IDENT>;
 close IDENT;
 if ($ident[1] ne $tmpid) {
  print $q->div( "Error: (Falsche TmpID in push.pl!)" ),
       $q->end_html;
  open(ERROR,">>data/error");
   print ERROR "$alias $tmpid $$ ".time." (Falsche TmpID in push.pl!) \n";
  close ERROR;
  exit 0;
 }
}

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
moves_steps    = 4;
moves_interval = 4;
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
<font size=1 color=ffffff face=arial>loading chat-engine @ PID: $pid...</font><br>
END
sleep 1;
print "<font size=1 color=ffffff face=arial>loading in progress ...</font><br>";
sleep 1;
print "<font size=1 color=ffffff face=arial><b>$alias</b>, welcome to <b>yChat</b>! :-)</font><br>";


if (-f "data/user/memos/$alias") {
 open(MEMOS,"<data/user/memos/$alias");
  my $memos = <MEMOS>;
 close MEMOS;
 print $memos;
 unlink("data/user/memos/$alias");
}

my $startime,$change_room,@flood;
OPENMSGFILE:
$change_room = 0;
$startime = time;

if (-e "data/msgs/$room") {
 open(MSGFILE,"<data/msgs/$room");
 unless (-d "data/online/pids") {
  mkdir "data/online/pids", 0777;
 } unless (-d "data/online/pids/$room") {
  mkdir "data/online/pids/$room", 0777;
 }
 open(PID,">data/online/pids/$room/$pid");
  print PID time;
 close PID;
} else {
 sleep 1;
 goto OPENMSGFILE;
}

&sendmsgs;
while (-f "data/online/users/$alias") {
 local $SIG{USR1} = \&sendmsgs;
 goto OPENMSGFILE if ($change_room == 1);
 sleep 15;
 print "\n";
}

sub sendmsgs {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
  my ($address, $stamp, $command, $action, $message) = split(/<;/, $_);
  if ($address eq "!" || $address eq $alias)  {
   if ($stamp > $startime) {
    if ($command eq "cr") {
     unlink("data/online/pids/$room/$pid");
     $room = $action;
     close MSGFILE;
     $change_room = 1;
    }
    print "$message<br>\n" if ($change_room != 1);
    push(@flood,$message);
    if ($flood[0] eq $flood[1] && $flood[0] eq $flood[2] && $flood[0] eq $flood[3]) {
     print "<i><font color=red> Jemand floodet den Chat !</font></i><br>\n";
    }
    shift(@flood) if ($#flood > 2);
   }
  }
 }
 seek(MSGFILE, $curpos, 0);
}

unlink("data/online/pids/$room/$pid");