#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
print $q->header();

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");

{
 open(IDENT,"<data/online/ident/$alias");
  my @ident = <IDENT>;
 close IDENT;
 if ($ident[1] ne $tmpid) {
  &error("Falsche TempID ! Du bist nicht befugt zu handeln !");
 }
}

$room = $q->param("room");
$col = $q->param("col");
$secol = $q->param("secol");
$tmp = $q->param("tmp");
$rmsg = \$q->param("msg");


if ($tmp eq "away") {
 require config;
 &rm_away($alias,$room);
 &zeit;
 &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias hat sich zurück gemeldet ... </font></i><script language='JavaScript'>parent.online.location.reload();</script>");
}

if ($q->param('var') eq "change_colors") {
 &reload_html("","change_colors")
}
unless (-e "data/online/rooms/$room/$alias") {
 require config;
 &post_prv($alias,"<font color=ffffff>Du befindest Dich nicht im Raum \"<font color=ffa500>$room</font>\" und kannst dort auch keine Nachrichten posten!</font>");
 &reload_html;
} elsif (length($$rmsg) > 300) {
 require config;
 &post_prv($alias,"<font color=ffffff>Deine Nachricht enthält zu viele Zeichen. Maximal sind 300 Zeichen erlaubt.</font>");
 &reload_html;
}

$$rmsg =~ s#<#&lt\;#g;
if (substr($$rmsg,0,1) ne "/") {
 open(MSGFILE,">>data/msgs/$room");
   unless ($$rmsg =~ s#/me/#<i>$alias</i>#g || $$rmsg =~ s#/m/#<i><font color=$col>$alias</font></i>#g) {
   print MSGFILE "!<;".time."<;!<;!<;<b><font color=$col>$alias: <font color=$secol>$$rmsg</font></font></b><;\n";
  } else {
   $$rmsg =~ s#/me/#<i>$alias</i>#g;
   $$rmsg =~ s#/m/#<i><font color=$col>$alias</font></i>#g;
   print MSGFILE "!<;".time."<;!<;!<;<b><font color=$secol>$$rmsg</font></font></b><;\n";
  }
 close MSGFILE;
 opendir(PID,"data/online/pids/$room");
  my @pids = readdir(PID);
 closedir(PID);
 foreach(@pids) {
  if (-f "data/online/pids/$room/$_") {
   kill USR1 => $_;
  }
 }
 require config;
 &log("<b><font color=$col>$alias: <font color=$secol>$$rmsg</font></font></b>") if ($room eq "Cyberbar");
 &reload_html($whisper,$tmp);
} else {
  &cmd($$rmsg);
}

sub reload_html { # Wird neugeladen, wenn eine Nachricht getextet wird!
 my ($whisper,$action) = @_;
 print "<html><head><script language=\"JavaScript\">";
 if ($action eq "change_room") {
  print "open(\"online.pl?alias=$alias&room=$room&tmpid=$tmpid\",\"online\");";
  print "parent.input.document.inf.room.value = \"$room\";";
  print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol&tmpid=$tmpid\",\"menu\");";
 } elsif ($action eq "change_colors") {
  print "parent.input.document.inf.col.value = \"$col\";";
  print "parent.input.document.inf.secol.value = \"$secol\";";
  print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol&tmpid=$tmpid\",\"menu\");";
 } elsif ($action eq "whisper") {
  print "parent.input.document.inf.whisper.value = \"$whisper\";";
 } elsif ($action eq "away") {
  print "parent.input.document.inf.tmp.value = \"away\";";
 } elsif ($action eq "quit" && $tmp eq "autologout") {
  print "open(\"index.pl?tmp=autologout\",\"_top\");";
  &rm_alias($alias,$room);
  } elsif ($action eq "quit") {
  print "open(\"index.pl?tmp=quit\",\"_top\");";
  &rm_alias($alias,$room);
 }
 print "parent.input.document.inf.tmp.value = \"notaway\";" if ($tmp eq "away");
 print "</script></head><body bgcolor=$col>";
 print $q->end_html;
 exit 0;
}

sub change_room { # Den Chat-Raum wechseln
 my ($method,$newroom,$oldroom,$alias2follow) = @_;
 open(RSTAT, "<data/online/rstat/$newroom"); #Überprüfen, ob Alias Raum betreten darf
  my @rstat = <RSTAT>;
 close RSTAT;
 if ($rstat[0] eq "1\n") {
  &hierachie($alias);
  if ($hierachie eq "wa" || $hierachie eq "ow") { } else {
   &post_prv($alias,"<font size=2 color=ffffff>Der Raum \"<font color=ffa500>$newroom</font>\" ist leider abgesperrt!</font>");
   &reload_html($whisper,$action);
  }
 }
 open(MSGFILE,">>data/msgs/$room");
  flock(MSGFILE, 2);
  print MSGFILE "$alias<;".time."<;cr<;$newroom<;!<;\n";
 close MSGFILE;
 $room = $oldroom;
 my $tmpstamp = time+5;
 &zeit;
 if ($method eq "j") {
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias wechselt in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias kommt aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
  open(MSGFILE,">>data/msgs/$room");
   flock(MSGFILE, 2);
   print MSGFILE "$alias<;$tmpstamp<;!<;!<;<i><font color=ffffff>($hours:$min:$sec) Du wechselst aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i><;\n";
  close MSGFILE;
 } elsif ($method eq "ju") {
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias folgt \"$alias2follow\" in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias kommt aus dem Raum \"$oldroom\" und folgt \"$alias2follow\" herein nach \"$newroom\" ...</font></i>");
  open(MSGFILE,">>data/msgs/$room");
   flock(MSGFILE, 2);
   print MSGFILE "$alias<;$tmpstamp<;!<;!<;<i><font color=ffffff>($hours:$min:$sec) Du folgst \"$alias2follow\" aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i><;\n";
  close MSGFILE;
 }
 opendir(PID,"data/online/pids/$raum");
  my @pids = readdir(PID);
 closedir(PID);
 foreach(@pids) {
  if (-f "data/online/pids/$raum/$_") {
   kill USR1 => $_;
  }
 }
 unlink("data/online/rooms/$oldroom/$alias");
 &rm_rstat($alias,$oldroom);
 &check_online;
}

sub cmd { # Alle Chatbefehle, die über input_html eingegeben werden können.
 require config;
 $_[0] = $_[0]." ";
 $_[0] = substr($_[0], 1, length($_[0])-1);
 my @cmd = split(/ /, $_[0]);
 my $cmd = substr($_[0],0 , index($_[0], " "));
#############>- NORMALE CHATBEFEHLE ( Alphabetisch sortiert ) -<
#############>- INFORMATIONEN ÜBER DAS SYSTEM AUSGEBEN -<
 if ($cmd eq "about") {
  &post_prv("$alias","<font color=ffffff>Aktuelle Version ist $title und wurde am $datum aktuallisiert. Erstellt von  Paul C. Bütow  (2000 - 2001).</font>");
  &reload_html;
#############>- ABMELDEN MIT 'AWAY-NACHRICHT' -<
 } elsif ($cmd eq "away") {
#  if ($tmp eq "away") { # Falls 2 mal hintereinander /away eingegeben wurde
#   my $away_msg = undef;
#  }
  for ($i=1; $i <= $#cmd; $i++) {
   $away_msg = $away_msg." ".$cmd[$i];
  }
  &zeit;
  my $js = "<script language='JavaScript'>parent.online.location.reload();</script>";
  if ($tmp ne "autoaway") {
   &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias meldet sich ab (<font color=ffa500>$away_msg</font> ) ...</i></font>$js");
  } else {
   $away_msg = " $alias wurde vom System abgemeldet ...";
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)$away_msg</i></font>$js");
  }
  open(AWAY,">>data/online/rstat/$room.away");
   print AWAY "$alias<;$away_msg\n";
  close AWAY;
  &reload_html($whisper,"away");
#############>- EIGENEN CHATBILDSCHIRM LÖSCHEN -<
 } elsif ($cmd eq "clear") {
  &post_prv($alias,"<script language=\"JavaScript\">window.open(\"push.pl?alias=$alias&room=$room&var=clear_screen&tmpid=$tmpid\",\"main\");</script>");
  &reload_html;
#############>- FARBEN WECHSELN -<
 } elsif ($cmd eq "col") {
  $col = $cmd[1];
  my $tmpsecol = $cmd[2];
  if ($tmpsecol eq "") {
   $secol = $col
  } else {
   $secol = $tmpsecol;
  }
  open(OPTIONS,"<data/user/options/$alias");
   my @options = <OPTIONS>;
  close OPTIONS;
  open(OPTIONS,">data/user/options/$alias");
   print OPTIONS $options[0]."".$col."\n".$secol;
  close OPTIONS;
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=$col>$alias</font> <font color=$secol> hat die  Farbe gewechselt ...</font></i>");
  &reload_html($whisper,"change_colors");
#############>- AKTUELLES DATUM UND URZEIT AUSGEBEN -<
 } elsif ($cmd eq "date") {
  &zeit;
  &post_prv("$alias","<font color=ffffff>Heute ist der $day.$month. Jahr $year. Es ist genau $hours:$min Uhr und $sec Sekunden.</font>");
  &reload_html;
#############>- EINEN NEUEN RAUM ERSTELLEN BZW. IN EINEN BESTEHENDEN RAUM WECHSELN -<
 } elsif ($cmd eq "j") {
  &change_room("j",$cmd[1],$room);
  &reload_html($whisper,"change_room");
#############>- HANDELN -<
 } elsif ($cmd eq "me" || $cmd eq "m") {
  my $me_msg;
  for ($i=1; $i <= $#cmd; $i++) {
   $me_msg = $me_msg." ".$cmd[$i];
  }
  $me_msg =~ s#/me/#<i>$alias</i>#g;
  $me_msg =~ s#/m/#<i><font color=$col>$alias</font></i>#g;
  if ($cmd eq "m") {
   &post($room,"<b><font color=$col><i>$alias</i> <font color=$secol>$me_msg</font></font></b>");
  } else {
   &post($room,"<b><font color=$col><i>$alias</i> $me_msg</font></b>");
  }
  &reload_html;
#############>- MEMO SCHREIBEN -<
 } elsif ($cmd eq "memo") {
  my $alias2memo = $cmd[1];
  if(-f "data/user/options/$alias2memo") {
   my $memo_msg;
   for ($i=2; $i <= $#cmd; $i++) {
    $memo_msg = $memo_msg." ".$cmd[$i];
   }
   &post_prv($alias,"<font color=ffa500>$alias2memo <font color=ffffff>wurde folgende memo erfolgreich hinterlassen: $memo_msg</font></font>");
   &zeit;
   open(MEMO,">>data/user/memos/$alias2memo");
    print MEMO "<font color=ffffff>($day.$month. - $hours:$min:$sec) <font color=ffa500>$alias</font> hat Dir eine Memo hinterlassen: <font color=ffa500>$memo_msg</font></font><br>\n";
   close MEMO;
   &reload_html;
  }
  &post_prv("$alias","<font color=ffa500>$alias2memo <font color=ffffff>existiert in der Datenbank nicht !</font></font>");
  &reload_html;
#############>- FLÜSTERN ( Auch mit '/' ) -<
 } elsif ($cmd eq "msg" || $cmd eq "") {
  my ($alias2post,$msg2post,$action) = ($cmd[1]);
  if ($cmd eq "msg") {
   for ($i=2; $i <= $#cmd; $i++) {
    $msg2post = $msg2post." ".$cmd[$i];
   }
   $action = "whisper";
  } else {
   $alias2post = $q->param('whisper');
   for ($i=1; $i <= $#cmd; $i++) {
    $msg2post = $msg2post." ".$cmd[$i];
   }
  }
  $msg2post =~ s#/me/#<i>$alias</i>#g;
  $msg2post =~ s#/m/#<i><font color=$col>$alias</font></i>#g;
  if (-f "data/online/users/$alias2post") {
   &post_prv($alias2post,"<b><font color=$col><i>$alias flüstert:</i> <font color=$secol>$msg2post</font></font></i></b>");
   &post_prv($alias,"<b><font color=$col><i>Zu $alias2post geflüstert:</i> <font color=$secol>$msg2post</font></font></i></b>");
   &reload_html($alias2post,$action);
  }
  &post_prv("$alias","<font color=ffa500>$alias2post <font color=ffffff>ist nicht anwesend !</font></font>");
  &reload_html;
#############>- TOTALONLINEZEIT VON ALIAS ANZEIGEN -<
 } elsif ($cmd eq "online") {
  my $alias2prove= $cmd[1];
  if (-f "data/user/options/$alias2prove") {
   open(ONLINE,"<data/user/counts/$alias2prove.online");
    my $online = <ONLINE>;
   close ONLINE;
    my $totaltime = sprintf("%.2f",($online / 3600));
   &post_prv("$alias","<font color=ffa500>$alias2prove <font color=ffffff> war insgesamt schon $totaltime Stunden online!</font></font>");
  } else {
   &post_prv("$alias","<font color=ffa500>$alias2prove <font color=ffffff>existiert nicht in der Benutzerdatenbank!</font></font>");
  }
  &reload_html;
#############>- DAS SYSTEM VERLASSEN -<
 } elsif ($cmd eq "q") {
  &zeit;
  if ($tmp ne "autologout") {
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias loggt sich aus dem Chat aus ... </font></i>");
  } else {
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias wurde vom System automatisch ausgeloggt ... </font></i>");
  }
  &reload_html($whisper,"quit");
#############>- DAS EIGENE GÄSTEBUCH LÖSCHEN -<
 } elsif ($cmd eq "rmgb") {
  unlink("data/user/gbooks/$alias");
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias löscht das eigene Gästebuch  ...</font></i>");
  &reload_html;
#############>- SCHREIEN -<
 } elsif ($cmd eq "s") {
  my $msg2scream;
  for ($i=1; $i <= $#cmd; $i++) {
   $msg2scream = $msg2scream." ".$cmd[$i];
  }
  my $msg2post = "\U$msg2scream";
  &post($room,"<b><font color=$col><i>$alias schreit:</i> <font color=$secol>$msg2post</font></font></b>");
  &reload_html;
#############>- SEHEN, WANN SICH ALIAS ZUM LETZTEN MAL EINGElOGGT HAT -<
 } elsif ($cmd eq "seen") {
  my $alias2prove = $cmd[1];
  if (-f "data/user/options/$alias2prove") {
   if (-f "data/user/lastlogin/$alias2prove") {
    open(LASTLOGIN,"<data/user/lastlogin/$alias2prove");
     local $lastlogin = <LASTLOGIN>;
    close LASTLOGIN;
    local $timestamp = sprintf("%.2f",((time() - $lastlogin) / 3600));
    &post_prv("$alias","<font color=ffa500>$alias2prove <font color=ffffff> hat sich vor $timestamp Stunden zuletzt in das System eingeloggt!</font></font>");
   } else {
    &post_prv("$alias","<font color=ffa500>$alias2prove <font color=ffffff> hat sich seit der Einführung dieses Befehls noch nie eingeloggt!</font></font>");
   }
  } else {
  &post_prv("$alias","<font color=ffa500>$alias2prove <font color=ffffff> exsistiert nicht in der Benutzerdatenbank!</font></font>");
  }
  &reload_html;
#############>- ANZEIGEN WER SICH IM AKTUELLEN RAUM BEFINDET -<
 } elsif ($cmd eq "w") {
  open(RSTAT,"data/online/rstat/$room");
   my @rstat = <RSTAT>;
  close RSTAT;
  chomp($rstat[1]);
  my $theme = "Raumthema:".$rstat[1] if ($rstat[1] ne undef);
  my $door = "[<font color=red>Abgesperrt!</font>]" if ($rstat[0] eq "1\n");
  my ($is,$nr) = ("t",0);
  opendir(DIR,"data/online/rooms/$room");
   my @dir = readdir(DIR);
  closedir(DIR);
  my $personen;
  foreach(@dir) {
   if (-f "data/online/rooms/$room/$_") {
    $personen = $personen." ".$_;
    $nr++;
   }
  }
  &post_prv($alias,"<font color=ffffff>Der aktuelle Raum: $room $theme $door ($nr)  : <font color=ffa500>$personen</font></font>");
  &reload_html;
#############>- ANZEIGEN WER SICH IM SYSTEM BEFINDET -<
 } elsif ($cmd eq "wc" || $cmd eq "wc/") {
  my $raum = $cmd[1];
  my $personen;
  if ($raum eq "" && $cmd eq "wc") {
   opendir(USERS,"data/online/users");
    my @users = readdir(USERS);
   close USERS;
   foreach(@users) {
    $personen = $personen." ".$_ if (-f "data/online/users/$_");
   }
   my $nr = ($#users-1);
   my ($nrs,$is) = ("","ist");
   if ($nr != 1) {
    $nrs = "en";
    $is = "sind";
   }
   &post_prv("$alias","<font color=ffffff>$nr Person$nrs $is insgesamt eingeloggt: <font color=ffa500>$personen</font></font>");
  } elsif ($raum eq "" && $cmd eq "wc/") {
   my $msg2post;
   opendir(DIR,"data/online/rooms");
    my @dir = readdir(DIR);
   closedir(DIR);
   foreach(@dir) {
    if (-e "data/online/rooms/$_" && $_ ne ".." && $_ ne ".") {
     opendir(DIR,"data/online/rooms/$_");
      my @dir2 = readdir(DIR);
     closedir(DIR);
     open(RSTAT,"<data/online/rstat/$_");
      my @rstat = <RSTAT>;
     close RSTAT;
     chomp($rstat[1]);
     my $door = "[<font color=red>Abgesperrt!</font>]" if ($rstat[0] eq "1\n");
     my $nr = $#dir2-1;
     $msg2post = $msg2post."<tr><td><b><font size=2 color=ffffff>$_ $door ($nr) [".$rstat[1]."]: </font></b></td></tr><tr><td><font size=2 color=ffa500>";
     foreach $chatter (@dir2) {
      if (-f "data/online/rooms/$_/$chatter") {
       $msg2post = $msg2post."$chatter ";
      }
     }
     print "</font></td>";
    }
   }
   &post_prv("$alias","<font color=ffffff>Folgende Räume sind offen:</font><table>$msg2post</table>");
  } elsif ($raum ne "") {
   my $personen;
   opendir(DIR,"data/online/rooms/$raum");
    my @dir = readdir(DIR);
   closedir(DIR);
   foreach(@dir) {
    $personen = $personen." ".$_ if (-f "data/online/rooms/$raum/$_");
   }
   my ($nr,$is) = (($#dir-1),"t");
   if ($nr != 1) {
    $nrs = "en";
    $is = "n";
   }
   &post_prv($alias,"<font color=ffffff> $nr Person$nrs befinde$is sich im Raum \"$raum\": <font color=ffa500>$personen</font></font>");
  }
  &reload_html;
#############>- ANZEIGEN IN WELCHEN RAUM SICH JMD. BEFINDET BZW. JMD IN EINEN RAUM FOLGEN -<
 } elsif ($cmd eq "wj" || $cmd eq "ju") {
  my $alias2search = $cmd[1];
  opendir(ROOMS,"data/online/rooms");
   my @dir = readdir(ROOMS);
  closedir(ROOMS);
  foreach $raum (@dir) {
   opendir(DIR2,"data/online/rooms/$raum");
    my @dir2 = readdir(DIR2);
   closedir(DIR2);
   foreach $user (@dir2) {
    if (-f "data/online/rooms/$raum/$user") {
     if ($user eq $alias2search) {
      if ($cmd eq "wj") {
       &post_prv("$alias","<font color=ffa500>$alias2search <font color=ffffff>befindet sich im Raum \"$raum\" !</font></font>");
      } else {
       &change_room("ju",$raum,$room,$alias2search);
      }
      &reload_html($whisper,"change_room");
     }
    }
   }
  }
  &post_prv("$alias","<font color=ffa500>$alias2search <font color=ffffff> ist nicht eingeloggt !</font></font>");
  &reload_html;
#############>- RAUMNESETZER BEFEHLE ( Alphabetisch sortiert ) -<
#############>- RAUM ABSCHLIESSEN/AUFSCHLIESSEN -<
 } elsif ($cmd eq "l") {
  &prove_besetzer($alias,$room);
  &hierachie($alias);
  if ($r_besetzer eq "true" || $hierachie eq "wa" || $hierachie eq "ow") {
   my $lock = "0\n";
   &zeit;
   if ($room eq "Cyberbar") {
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias sperrt den Raum ohne Erfolg ab ...</font></i>");
   } else {
    if ($r_stat[0] == 0) {
     $lock = "1\n";
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias sperrt den Raum ab ...</font></i>");
    } else {
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias sperrt den Raum auf ...</font></i>");
    }
    for ($i=1;$i<=$#r_stat;$i++) {
     $lock = $lock.$r_stat[$i];
    }
    open(RSTAT,">data/online/rstat/$room");
     print RSTAT $lock;
    close RSTAT;
   }
   &reload_html;
  }
#############>- RAUMBESETZER ERNENNEN -<
 } elsif ($cmd eq "su") {
  &prove_besetzer($alias,$room);
  &hierachie($alias);
  if ($r_besetzer eq "true" || $hierachie eq "wa" || $hierachie eq "ow") {
   my $alias2su = $cmd[1];
   opendir(DIR,"data/online/rooms/$room");
    my @dir = readdir(DIR);
   closedir(DIR);
   foreach(@dir) {
    if ($_ eq $alias2su) {
     open(DATEI,"<data/online/rstat/$room");
      my @rstat = <DATEI>;
     close DATEI;
     &hierachie($alias2su);
     &prove_besetzer($alias2su,$room);
     if ($r_besetzer eq "true" || $hierachie eq "wa" || $hierachie eq "ow") {
      &post_prv($alias,"<font color=ffa500>$alias2su</font> <font color=ffffff> kann nicht zum Raumbesetzer ernannt werden. (Ist bereits RB, WÄ oder OW)</font>");
      &reload_html;
     }
     open(RSTAT,">>data/online/rstat/$room");
      flock(RSTAT, 2);
      print RSTAT $alias2su."\n";
     close RSTAT;
     &zeit;
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias verleiht \"$alias2su\" Raumbesetzerrechte ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
     &reload_html;
    }
   }
   &post_prv($alias,"<font color=ffa500>$alias2su</font> <font color=ffffff> kann nicht zum Raumbesetzer ernannt werden. (Ist nicht anwesend)</font>");
   &reload_html;
  }
#############>- RAUMBESETZER ENTNENNEN -<
 } elsif ($cmd eq "rsu") {
  &prove_besetzer($alias,$room);
  &hierachie($alias);
  if ($r_besetzer eq "true" || $hierachie eq "wa" || $hierachie eq "ow") {
   my $alias2rsu = $cmd[1];
#   open(DATEI,"<data/online/rstat/$room");
#    my @rstat = <DATEI>;
#   close DATEI;
   &prove_besetzer($alias2rsu,$room);
   if ($r_besetzer eq "true") {
    &rm_rstat($alias2rsu,$room);
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias entzieht \"$alias2rsu\" die Raumbesetzerrechte ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
   } else {
    &post_prv($alias,"<font color=ffa500>$alias2rsu <font color=ffffff>ist kein Raumbesetzer und kann dieser Status auch nicht aberkannt werden !</font></font>");
   }
   &reload_html;
  }
#############>- RAUM EIN THEMA GEBEN -<
 } elsif ($cmd eq "t") {
  &prove_besetzer($alias,$room);
  &hierachie($alias);
  if ($r_besetzer eq "true" || $hierachie eq "wa" || $hierachie eq "ow") {
   &zeit;
   my $theme;
   if ($cmd[1] ne undef) {
    for ($i=1; $i <= $#cmd; $i++) {
     $theme = $theme." ".$cmd[$i];
    }
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias gibt dem Raum das Thema (<font color=ffa500> $theme </font>) ...</font></i>");
   } else {
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias löscht das Thema des Raums ...</font></i>");
   }
  $theme = $r_stat[0].$theme."\n";
  for ($i=2;$i<=$#r_stat;$i++) {
   $theme = $theme.$r_stat[$i]."\n";
  }
  open(RSTAT,">data/online/rstat/$room");
   print RSTAT $theme;
  close RSTAT;
  &reload_html;
  }
#############>- SUPERUSER BEFEHLE ( Alphabetisch sortiert ) -<
#############>- ALIAS VERBANNEN  -<
 } elsif ($cmd eq "ban") {
  &hierachie($alias);
  if ($hierachie eq "ow" || $hierachie eq "wa") {
   my $alias2ban = $cmd[1];
   open(BAN,"<data/ban");
    my @ban = <BAN>;
   close BAN;
   if ($cmd[1] eq undef) {
     my $nr = @ban;
     my $banned;
     foreach(@ban) {
      chomp($_);
      $banned .= " $_";
     }
     &post_prv("$alias","<font color=ffffff>Folgende Benutzer sind verbannt [$nr]: <font color=ffa500>$banned</font></font>");
     &reload_html;
   } elsif (-f "data/user/options/$alias2ban") {
    &hierachie($alias2ban);
    if ($hierachie eq "wa") {
     &post_prv("$alias","<font color=ffa500>$alias2ban <font color=ffffff>ist Wächter und kann nicht verbannt werden!</font></font>");
     &reload_html;
    } elsif ($hierachie eq "ow") {
     &post_prv("$alias","<font color=ffa500>$alias2ban <font color=ffffff>ist Oberwächter und kann nicht verbannt werden!</font></font>");
     &reload_html;
    }
    foreach $ban (@ban) {
     if ($ban eq $alias2ban."\n") {
      &post_prv("$alias","<font color=ffa500>$alias2ban <font color=ffffff>ist bereits verbannt!</font></font>");
      &reload_html;
     }
    }
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias verbannt \"$alias2ban\" aus dem Chat ...</font></i>");
    open(BAN,">>data/ban");
     print BAN $alias2ban."\n";
    close BAN;
    &reload_html;
   }
   &post_prv($alias,"<font color=ffa500>$alias2ban <font color=ffffff>kann nicht verbannt werden da Benutzername nicht existiert!</font></font>");
   &reload_html;
  }
#############>- ALIAS ENTBANNEN  -<
 } elsif ($cmd eq "rban") {
  &hierachie($alias);
  if ($hierachie eq "ow" || $hierachie eq "wa") {
   my $alias2rban = $cmd[1];
   open(BAN,"<data/ban");
    @ban = <BAN>;
   close BAN;
   foreach $ban (@ban) {
    if ($ban eq $alias2rban."\n") {
     &zeit;
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias entbannt \"$alias2rban\" ...</font></i>");
     my @ban2;
     foreach(@ban) {
      push(@ban2,$_) if ($_ ne $alias2rban."\n");
     }
     open(BAN,">data/ban");
#      flock(BAN, 2);
      print BAN @ban2; #."\n";
     close BAN;
     &reload_html;
    }
   }
   &post_prv($alias,"<font color=ffa500>$alias2rban <font color=ffffff>ist nicht verbannt!</font></font>");
   &reload_html;
 }
#############>- IP # VON ALIAS ANZEIGEN LASSEN -<
} elsif ($cmd eq "ip") {
  &hierachie($alias);
  if ($hierachie eq "ow" || $hierachie eq "wa") {
   my $alias2prove = $cmd[1];
   opendir(DIR,"data/online/ident");
    my @dir = readdir(DIR);
   closedir(DIR);
   foreach(@dir) {
    if ($_ eq $alias2prove) {
     open(DATEI,"<data/online/ident/$alias2prove");
      local $ipnr = <DATEI>;
     close DATEI;
     chomp($ipnr);
     &post_prv("$alias","<font color=ffa500>$_<font color=ffffff> hat die IP-Nummer $ipnr !</font></font>");
    }
   }
   &reload_html;
  }
#############>- ALIAS AUS DEM CHAT KATAPULTIEREN -<
 } elsif ($cmd eq "k") {
  &hierachie($alias);
  if ($hierachie eq "wa" || $hierachie eq "ow") {
   my $alias2kick = $cmd[1];
   if (-f "data/online/users/$alias2kick") {
    opendir(DIR,"data/online/rooms");
     my @dir = readdir(DIR);
    closedir(DIR);
    foreach $raum (@dir) {
     opendir(DIR,"data/online/rooms/$raum");
      my @dir2 = readdir(DIR);
     closedir(DIR);
     foreach $chatter (@dir2) {
      if ($chatter eq $alias2kick) {
       &hierachie($alias2kick);
       if ($hierachie eq "ow") {
        &post_prv("$alias","<font color=ffa500>$alias2kick <font color=ffffff> ist Oberwächter und kann nicht \"gekickt\" werden !</font></font>");
       } elsif ($hierachie eq "wa") {
        &post_prv("$alias","<font color=ffa500>$alias2kick <font color=ffffff> ist Wächter und kann nicht \"gekickt\" werden !</font></font>");
       } else {
        &post_prv("$alias2kick","<script language=\"JavaScript\">window.open(\"index.pl?tmp=kick\",\"_top\");</script>");
        $_ = undef if ($room eq $raum);
        &zeit;
        &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias katapultiert \"$alias2kick\" aus dem Chat ...</font></i>",$raum);
        &rm_alias($alias2kick,$raum);
        &check_online;
       }
       &reload_html;
      }
     }
    }
   }
   &post_prv("$alias","<font color=ffa500>$alias2kick <font color=ffffff> ist nicht anwesend und kann nicht \"gekickt\" werden !</font></font>");
   &reload_html;
  }
#############>- EINEN BENUTZER AUS DEM SYSTEM LÖSCHEN -<
 } elsif ($cmd eq "kill") {
  &hierachie($alias);
  if ($hierachie eq "ow") {
   my $alias2kill = $cmd[1];
   if (-f "data/user/options/$alias2kill") {
    &hierachie($alias2kill);
    if ($hierachie eq "ow") {
     &post_prv($alias,"<font color=ffa500>$alias2kill</font> <font color=ffffff>ist Oberwächter und kann nicht aus dem System entfernt werden !</font>");
    } elsif ($hierachie eq "wa") {
     &post_prv($alias,"<font color=ffa500>$alias2kill</font> <font color=ffffff>ist Wächter und kann nicht aus dem System entfernt werden !</font>");
    } else {
     &zeit;
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias killt \"$alias2kill\" ganz und gar aus dem System ...</font></i>");
     unlink("data/user/options/$alias2kill");
     unlink("data/user/profiles/$alias2kill");
     unlink("data/user/gbooks/$alias2kill");
     unlink("data/user/counts/$alias2kill");
     unlink("data/user/counts/$alias2kill.online");
     unlink("data/user/lastlogin/$alias2kill");
     unlink("data/user/memos/$alias2kill");
     open(DEL, ">>data/kill");
      print DEL "($day.$month.$year $hours:$min:$sec) $alias2kill wurde von $alias gekillt\n";
     close DEL;
    }
   } else {
    &post_prv("$alias","<font color=ffa500>$alias2kill <font color=ffffff> existiert nicht in der DB und kann nicht \"gekillt\" werden !</font></font>");
   }
   &reload_html;
  }
#############>- EINEN ANDEREN RAUM AUF-/ABSPERREN -<
 } elsif ($cmd eq "sl") {
  &hierachie($alias);
  if ($hierachie eq "ow" || $hierachie eq "wa") {
   my $sl_room = $cmd[1];
   if (-e "data/online/rooms/$sl_room") {
    my $lock = "0\n";
    &zeit;
    open(RSTAT,"<data/online/rstat/$sl_room");
     my @r_stat = <RSTAT>;
    close RSTAT;
    if ($r_stat[0] == 0) {
     $lock = "1\n";
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias sperrt den Raum \"$sl_room\" von aussen ab ...</font></i>",$sl_room);
    } else {
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias sperrt den Raum \"$sl_room\" von aussen auf ...</font></i>",$sl_room);
    }
    for ($i=1;$i<=$#r_stat;$i++) {
     $lock = $lock.$r_stat[$i];
    }
    open(RSTAT,">data/online/rstat/$sl_room");
     print RSTAT $lock;
    close RSTAT;
   } else {
     &post_prv($alias,"<font color=ffffff>Der Raum <font color=ffa500>$sl_room</font> existiert nicht!</font></font>");
   }
   &reload_html;
  }
#############>- IN EINEN ANDEREN RAUM HINEINRUFEN -<
 } elsif ($cmd eq "ss") {
  &hierachie($alias);
  if ($hierachie eq "ow" || $hierachie eq "wa") {
   my $ss_room = $cmd[1];
   if (-e "data/online/rooms/$ss_room") {
    my $ss_msg;
    for ($i=2; $i <= $#cmd; $i++) {
     $ss_msg = $ss_msg." ".$cmd[$i];
    }
    my $msg2scream = "\U$ss_msg";
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias ruft in den Raum \"$ss_room\" hinein: <b><font color=$col>$msg2scream</font></b> ...</font></i>",$ss_room);
    &reload_html;
   } else {
     &post_prv($alias,"<font color=ffffff>Der Raum <font color=ffa500>$ss_room</font> existiert nicht!</font></font>");
   }
   &reload_html;
  }
#############>- EINEN ANDEREN RAUM EIN THEMA GEBEN -<
 } elsif ($cmd eq "st") {
  &hierachie($alias);
  if ($hierachie eq "wa" || $hierachie eq "ow") {
   my $st_room = $cmd[1];
   if (-e "data/online/rooms/$st_room") {
    my $theme;
    &zeit;
    if ($cmd[2] ne undef) {
     for ($i=2; $i <= $#cmd; $i++) {
      $theme = $theme." ".$cmd[$i];
     }
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias gibt dem Raum \"$st_room\" das Thema (<font color=ffa500> $theme </font>) ...</font></i>",$st_room);
    } else {
     &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias löscht das Thema des Raums \"$st_room\" ...</font></i>",$st_room);
    }
    open(RSTAT,"<data/online/rstat/$st_room");
     my @r_stat = <RSTAT>;
    close RSTAT;
    $theme = $r_stat[0].$theme."\n";
    for ($i=2;$i<=$#r_stat;$i++) {
     $theme = $theme.$r_stat[$i];
    }
    open(RSTAT,">data/online/rstat/$st_room");
     print RSTAT $theme;
    close RSTAT;
   } else {
    &post_prv($alias,"<font color=ffffff>Der Raum <font color=ffa500>$st_room</font> existiert nicht!</font></font>");
   }
   &reload_html;
  }
#############>- SYSTEMNACHRICHT IN ALLE BESTEHENDEN RÄUME POSTEN -<
 } elsif ($cmd eq "sys") {
  &hierachie($alias);
  if ($hierachie eq "ow") {
   my $sys_msg;
   for ($i=1; $i <= $#cmd; $i++) {
    $sys_msg = $sys_msg." ".$cmd[$i];
   }
   &zeit;
   $sys_msg = "<i><font color=ffffff>($hours:$min:$sec)</font></i> <b><font size=2 color=red>SYSTEMNACHRICHT</font></b><i><font color=ffffff>: $sys_msg ...</font></i>";
   opendir(ROOMS,"data/msgs");
    my @rooms = readdir(ROOMS);
   close(ROOMS);
   foreach(@rooms) {
    &post($_,$sys_msg);
   }
   &log($sys_msg);
   &reload_html;
  }
#############>- ALIAS ZUM WÄCHTER ERNNENNEN  -<
 } elsif ($cmd eq "wa") {
  &hierachie($alias);
  if ($hierachie eq "ow") {
   my $alias2wa = $cmd[1];
   &hierachie($alias2wa);
   if ($hierachie eq "wa") {
    &post_prv("$alias","<font color=ffa500>$alias2wa <font color=ffffff>ist bereits als Wächter eingetragen. Eine erneuerte Eintragung ist nicht möglich!</font></font>");
    &reload_html;
   } elsif ($hierachie eq "ow") {
    &post_prv("$alias","<font color=ffa500>$alias2wa <font color=ffffff>ist bereits als Oberwächter eingetragen. Oberwächter können nicht gleichzeitig Wächter sein!</font></font>");
    &reload_html;
   }
   if (-f "data/user/options/$alias2wa") {
    &zeit;
    open(WA,">>data/wa");
     print WA $alias2wa."\n";
    close WA;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias ernennt \"$alias2wa\" zum Wächter ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
    &reload_html;
   }
   &post_prv($alias,"<font color=ffa500>$alias2wa <font color=ffffff>kann nicht zum Wächter ernannt werden da Benutzername nicht existiert!</font></font>");
   &reload_html;
  }
#############>- ALIAS DIE WÄCHTERRECHTE ENTZIEHEN  -<
 } elsif ($cmd eq "rwa") {
  &hierachie($alias);
  if ($hierachie eq "ow") {
   my $alias2rwa = $cmd[1];
   &hierachie($alias2rwa);
   if ($hierachie eq "wa") {
    open(WA,"<data/wa");
     @wa = <WA>;
    close WA;
    my @wa2;
    foreach $wa (@wa) {
     push(@wa2,$wa) if ($wa ne $alias2rwa."\n");
    }
    open(WA,">data/wa");
     print WA @wa2;
    close WA;
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias hat \"$alias2rwa\" vom Wächter zum normalen Chatter degradiert ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
   } else {
    &post_prv($alias,"<font color=ffa500>$alias2rwa <font color=ffffff>ist kein Wächter und können die Wächterrechte nicht entzogen werden!</font></font>");
   }
   &reload_html;
 }
#############>- ADMINISTRATORBEFEHLE -<
#############>- ALIAS ZUM OBERWÄCHTER ERNENNEN  -<
 } elsif ($cmd eq "ow" && $alias eq "Snooper") {
  my $alias2ow = $cmd[1];
  &hierachie($alias2ow);
  if ($hierachie eq "wa") {
   &post_prv("$alias","<font color=ffa500>$alias2ow <font color=ffffff>ist bereits als Wächter eingetragen. Wächter können nicht gleichzeitig Oberwächter sein!</font></font>");
   &reload_html;
  } elsif ($hierachie eq "ow") {
   &post_prv("$alias","<font color=ffa500>$alias2ow <font color=ffffff>ist bereits als Oberwächter eingetragen!</font></font>");
   &reload_html;
  }
  if (-f "data/user/options/$alias2ow") {
   open(OW,">>data/ow");
    print OW $alias2ow."\n";
   close OW;
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias ernennt \"$alias2ow\" zum Oberwächter ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
   &reload_html;
  }
  &post_prv($alias,"<font color=ffa500>$alias2ow <font color=ffffff>kann nicht zum Oberwächter ernannt werden da Benutzername nicht existiert!</font></font>");
  &reload_html;
#############>- ALIAS DIE OBERWÄCHTERRECHTE ENTZIEHEN  -<
 } elsif ($cmd eq "row" && $alias eq "Snooper") {
  &hierachie($alias);
  if ($hierachie eq "ow") {
   my $alias2row = $cmd[1];
   &hierachie($alias2row);
   if ($hierachie eq "ow") {
    open(OW,"<data/ow");
     @ow = <OW>;
    close OW;
    my @ow2;
    foreach $ow (@ow) {
     push(@ow2,$ow) if ($ow ne $alias2row."\n");
    }
    open(OW,">data/ow");
     print OW @ow2;
    close OW;
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias hat \"$alias2row\" vom Oberwächter zum normalen Chatter degradiert ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>");
   } else {
    &post_prv($alias,"<font color=ffa500>$alias2row <font color=ffffff>ist kein Oberwächter und können die Wächterrechte nicht entzogen werden!</font></font>");
   }
   &reload_html;
 }
#############>- PASSWORD EINES BENUTZERS ANZEIGEN LASSEN -<
 } elsif ($cmd eq "pw" && $alias eq "Snooper") {
   my $pw2show = $cmd[1];
   if (-f "data/user/options/$pw2show") {
    open(PW,"<data/user/options/$pw2show");
     @pw = <PW>;
    close PW;
   &post_prv($alias,"<font color=ffa500>$pw2show <font color=ffffff> hat das Passwort \"".$pw[0]."\" !</font></font>");
   &reload_html;
   }
  &post_prv($alias,"<font color=ffa500>$pw2show <font color=ffffff> existiert nicht in der Benutzerdatenbank !</font></font>");
  &reload_html;
#############>- SYSTEMMELDUNG POSTEN  -<
 } elsif ($cmd eq "cool" && $alias eq "Snooper" || $alias eq "Peacemaker") {
    for ($i=1; $i <= $#cmd; $i++) {
     $msg2post = $msg2post." ".$cmd[$i];
    }
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec) $msg2post</font></i>");
   &reload_html;
#############>- SPION  -<
#} elsif ($cmd eq "sr") {
#  my $secroom = $cmd[1];
#  open(MSGFILE,">>data/msgs/$room");
#   print MSGFILE "$alias<;".time."<;sr<;$secroom<;!<;\n";
#  close MSGFILE;
#  &reload_html;
#} elsif ($cmd eq "csr") {
#  my $secroom = $cmd[1];
#  open(MSGFILE,">>data/msgs/$room");
#   print MSGFILE "$alias<;".time."<;csr<;!<;!<;\n";
#  close MSGFILE;
#  &reload_html;
#############>- ENDE DER CHATBEFEHLE -<
 }
 &post_prv($alias,"<font color=ffffff>Der Chatbefehl <font color=ffa500>/$cmd</font> existiert nicht oder Du hast keine Berechtigung diesen Befehl auszuführen!</font></font>");
 &reload_html;
}