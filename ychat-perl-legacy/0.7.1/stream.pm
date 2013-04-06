sub stream {

# chomp $room;
 &secure_checkid($alias,$tmpid);

 my $pid = $$;
 $SIG{CHLD} = 'IGNORE';
 $|++;


  open RSTAT, "<data/online/rstat/$room";
   my @rstat = <RSTAT>;
  close RSTAT;

  #Überprüfen, ob Alias Raum betreten darf
  if ($rstat[0] eq "1\n") {
   if (3 <= &hierachie($alias)) {
    print "<font color='ffffff'>Der Raum <font color='ffa500'>$room</font> ist leider abgesperrt! Du wirst in den Ram <b>$standardroom</b> umgeleitet ...</font>";
    $room = $standardroom;
   }
  }


print <<END;
<html>
 <head>
<script language="JavaScript">
<!--
 open('chat?request=online&room=$room&alias=$alias&tmpid=$tmpid','online');
 open('chat?request=input&alias=$alias&room=$room&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','input');

// open('chat?request=input&alias=$alias&room=$room&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','input');

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
<body bgcolor="$chatbg">
END
unlink("data/online/rooms/$loginplace/$alias");
&check_online;

print "<font size=1 color=ffffff face=arial>$title - Letzte &Auml;nderung $datum\n; Der Chat wird geladen...\n<br>";
sleep 1;
print "<b>Hallo <font color=$col>$alias</font></b>, mit dem Befehl /menu kannst du das Chat-Men&uuml; aufrufen!<br>";
sleep 1;
print "Du befindest dich im Raum <b>$room</b>: ".get_theme($room)."<br>\n";
print "</font>\n";

 if (-f "data/user/memos/$alias") {
  open(MEMOS,"<data/user/memos/$alias");
   my @memos = <MEMOS>;
  close MEMOS;
  my $memos;

  foreach (@memos) {
   $memos .= $_;
  }

  print $memos;
  unlink("data/user/memos/$alias");
 }

OPENMSGFILE:
 my $change_room = 0;
 my $startime = time;
 my $flooding = 0;
 my @ignore;
 my @flood;


 unless (-f "data/online/rstat/$room") {
  open(DATEI,">data/online/rstat/$room"); # Raumbesetzerstatus setzen
   print DATEI "0\n\n$alias<;1\n";
  close DATEI;
 }

 open(DATEI,">data/online/users/$alias");
  print DATEI time;
 close DATEI;

 open(DATEI,">data/online/rooms/$room/$alias");
  print DATEI time;
 close DATEI;

 if (-e "data/msgs/$room") {
  open(MSGFILE,"<data/msgs/$room");

  mkdir "data/online/pids", 0777
   unless (-d "data/online/pids");

  mkdir "data/online/pids/$room", 0777
   unless (-d "data/online/pids/$room");

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

  goto OPENMSGFILE
   if ($change_room == 1);

  sleep 15;
  print "\n";
 }

 sub sendmsgs {
 for ($curpos = tell(MSGFILE); <MSGFILE>; $curpos = tell(MSGFILE)) {
  my ($address, $stamp, $command, $action, $message, $from) = split(/<;/, $_);

  if ($address =~ /$\!/ || $address eq $alias)  {
   if ($stamp > $startime) {

    # Falls die Farben gewechselt wurden
    if ($command eq "col") {
     ($col, $secol) = split ";", $action;

    } elsif ($command eq "cr") {
     unlink("data/online/pids/$room/$pid");
     $room = $action;
     close MSGFILE;
     $change_room = 1;

    # Falls jemand aus der Ignore-Liste eingetragen, oder ausgetragen werden soll
    } elsif ($command eq "ig") {
     my @temp;
     my $add2list = 1;

     foreach (@ignore) {
      if ($action ne $_) {
       push @temp, $_;

      } else {
       $add2list = 0;
      }
     }

     if ($add2list == 1) {
      push @ignore, $action;
      print "<font color='#ffffff'><font color='#ffa500'>$action</font> wird nun ignoriert!</font><br>\n";
      &post_prv_extra($action,"<font color='#ff0000'>Du wirst nun von $alias ignoriert!</font>")

     } else {
      @ignore = @temp;
      print "<font color='#ffffff'><font color='#ffa500'>$action</font> wird nun nicht mehr ignoriert!</font><br>\n";
      &post_prv_extra($action,"<font color='#ff0000'>Du wirst nun von $alias nicht mehr ignoriert!</font>")
     }

    # Falls Liste aller ignorierten Nicknames angezeigt werden soll
    } elsif ($command eq "igl") {
     my $ignored;
     $ignored .= "$_ " foreach (@ignore);

     if ($#ignore == -1) {
      print "<font color='#ffffff'>Momentan wird keine Person von dir ignoriert!</font><br>\n";

     } else {
      print "<font color='#ffffff'>Folgende Personen werden von dir ignoriert: <font color='#ffa500'>$ignored</font>!</font><br>\n";
     }

    # Falls User den Chat, aber nicht das System verlassen will
    } elsif ($command eq "quit") {
     print "<script language='JavaScript'>\n";
     print "open('chat?request=online&room=$loginplace&alias=$alias&tmpid=$tmpid','online');\n";
     print "open('chat?request=htdocs','input');\n";
     print "open('chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','main');\n";
     print "</script>\n";
     last;

    # Nachricht ausgeben falls nicht der Raum gewechselt wird
    } elsif ($change_room != 1) {

     # Wenn der Benutzer ignoriert wird, die nachricht nicht ausgeben
     my $ignored = 0;
     foreach (@ignore) {
      if ($_ eq $from) {
       $ignored = 1;
       last;
      }
     }

     if ($ignored != 1) {
      print "$message\n";

     } else {
      $address =~ s/\!//;

      &post_prv_extra($from,"<font color='#ff0000'>Du wirst von $alias ignoriert!</font>")
       if ($address eq $alias);
     }
    }

    push(@flood,$address.$command.$action.$message.$from);

    # Falls Jemand den Chat flutet
    if ($flood[0] eq $flood[1] && $flood[0] eq $flood[2] && $flood[0] eq $flood[3]) {
     print "<i><font color=red>$from flutet den Chat !</font></i><br>\n";

     if ($from eq $alias && $flooding++ && $flooding >= 4) {
      print "<script language='JavaScript'>open('chat?request=start&tmp=autokick','_top');</script>\n";
      &rm_alias($alias,$room);
      &post($room,"<i><font color='ffffff'>".&timestring." <font color='ffa500'><b>Das System</b></font> katapultiert <b>$alias</b> wegen Fluten des Chats nach draussen ... </font></i>", $alias);
     }

    } else {
     $flooding = 0;
    }

    shift(@flood) if ($#flood > 2);
   }
  }
 }

 seek(MSGFILE, $curpos, 0);
}

unlink("data/online/pids/$room/$pid");
}

sub post_prv_extra {
 my ($alias,$message) = @_;
 my $helpid = fork();

 if ($helpid == 0) {
  &post_prv($alias,$message);
  exit 0;
 }
}

1;
