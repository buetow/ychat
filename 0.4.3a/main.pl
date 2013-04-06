#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;          # CGI-Modul laden

$q = new CGI;

require config;         # config.pm in Dokument laden

print
 $q->header();          # HTTP-Header erzeugen

#####################################
##WELCHE SUB MUß AUSGEFÜHRT WERDEN?##
#####################################
$var = $q->param("var");
$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
&secure_checkid($alias);
$room = $q->param("room");
$col = $q->param("col");
$secol = $q->param("secol");
$tmp = $q->param("tmp");
if ($var eq "input_html") {
 &input_html;
} elsif ($var eq "post_msg") {
 $tmp = $q->param("tmp");
 $msg = $q->param("msg");
 $whisper = $q->param("whisper");
 &post_msg;
} else {
 &error("Parameter fehlt");
}

sub input_html { # Eingabemaske für den Chat.
 if ($tmp eq "login") {
  &remove_rooms;
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat den Chat betreten ...</font></i>");
 }
 my $whisper = @_;
 $whisper = $q->param('whisper') if ($whisper == 0);
print <<END;
<html>
 <head>
  <script language="JavaScript">

maxVerlauf = 10;  // Anzahl der Einträge des Verlaufs
maxPreview = 20;  // Zeichenlänge der Verlaufsvorschau
fullVerlauf = new Array( maxVerlauf );

function delout()
{
  addVerlauf( document.inf.msg.value );
    document.inf.submit();  // Formular absenden
    document.inf.msg.focus();  // Fukus in das Eingabeform
    document.inf.msg.select();  // Text im Eigabeform markieren
  return false;
}
function CheckAuswahl()
{
  id = document.verlaufform.verlauf.selectedIndex;

  if( fullVerlauf[ id-1 ] == null )  // Falls das Feld leer ist
    return false;  // Aufruf ignorieren

  document.inf.msg.value = fullVerlauf[ id-1 ];
  document.inf.msg.focus();

  return true;
}
function addVerlauf( msg )
{
  if( msg == "" )  // Falls msg leer
    return false;  // Aufruf ignorieren

  for( i=0; i<(maxVerlauf-1); i++ )  // Alle Einträge einen aufrutschen
  {
    document.verlaufform.verlauf.options[i+1].text = document.verlaufform.verlauf.options[i+2].text;
    fullVerlauf[i] = fullVerlauf[i+1];
  }
  if( msg.length > maxPreview )
    pre = msg.slice( 0, maxPreview-3 ) + "...";
  else
    pre = msg;

  document.verlaufform.verlauf.options[ maxVerlauf ].text = pre;  // Neue Vorschau anfügen
  fullVerlauf[ maxVerlauf-1 ] = msg;  // Neuen Eintrag anfügen

//  document.verlaufform.verlauf.selectedIndex = maxVerlauf;  // Fokus auf den letzten Eintrag
  document.verlaufform.verlauf.selectedIndex = 0;  // Fokus auf den ersten Eintrag (Überschrift)
}

  </script>
$style
 </head>
<body class="blank" onload="document.inf.msg.focus();">
 <div align="left">
  <table border="0" cellpadding="0" cellspacing="0">
   <tr>
    <td>
     <form name="inf" target="reload" action="main.pl" method="GET" onsubmit="return delout()">
      <input type="hidden" name="var" value="post_msg">
      <input type="hidden" name="alias" value="$alias">
      <input type="hidden" name="room" value="$room">
      <input type="hidden" name="col" value="$col">
      <input type="hidden" name="secol" value="$secol">
      <input type="hidden" name="tmpid" value="$tmpid">
      <input type="hidden" name="whisper" value="$whisper">
      <input type="hidden" name="tmp" value="$tmp">
      <input size="50" name="msg">
     </form>
    </td>
    <td>
     <form name="verlaufform">
      <select size="1" name="verlauf" size="40" onChange="CheckAuswahl()">
       <option value>- Nachrichten-Verlauf -</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
      </select>
     </form>
    </td>
   </tr>
  </table>
 </div>
</body>
</html>
END
}

sub reload_html { # Wird neugeladen, wenn eine Nachricht getextet wird!
 my ($whisper,$action) = @_;
 print "<html><head><script language=\"JavaScript\">";
 if ($action eq "change_room") {
  print "open(\"online.pl?alias=$alias&room=$room&tmpid=$tmpid\",\"online\");";
  print "open(\"main.pl?var=input_html&alias=$alias&room=$room&col=$col&secol=$secol&whisper=$whisper&tmpid=$tmpid\",\"input\");";
  print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol&tmpid=$tmpid\",\"menu\");";
 } elsif ($action eq "change_colors") {
  print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol&tmpid=$tmpid\",\"menu\");";
  print "open(\"main.pl?var=input_html&alias=$alias&room=$room&col=$col&secol=$secol&whisper=$whisper&tmpid=$tmpid\",\"input\");";
 } elsif ($action eq "whisper") {
  print "open(\"main.pl?var=input_html&alias=$alias&col=$col&secol=$secol&room=$room&whisper=$whisper&tmpid=$tmpid\",\"input\");";
 } elsif ($action eq "away") {
  print "open(\"main.pl?var=input_html&alias=$alias&col=$col&secol=$secol&room=$room&tmp=noaway&whisper=$whisper&tmpid=$tmpid\",\"input\");";
 } elsif ($action eq "noaway") {
  &zeit;
  &post("$room,<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias meldet sich zurück ... </font></i>");
  print "open(\"main.pl?var=input_html&alias=$alias&col=$col&secol=$secol&room=$room&whisper=$whisper&tmpid=$tmpid\",\"input\");";
 } elsif ($action eq "quit") {
  print "open(\"index.pl?tmp=quit\",\"_top\");";
 }
 print "</script></head><body bgcolor=$col>";
 print
 $q->end_html;
}

sub post_msg { # Wird aufgerufen, wenn aus input_html eine Eingabe gepostet wurde.
  if ($tmp eq "noaway") {
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat sich zurück gemeldet ... </font></i>");
   &post_prv($alias,"<script language=\"JavaScript\">open(\"main.pl?var=input_html&alias=$alias&col=$col&secol=$secol&room=$room&whisper=$whisper&tmpid=$tmpid\",\"input\");</script>");
  }
 $msg =~ s#<#&lt\;#g;		# HTML-Tags deaktivieren
 if (substr($msg,0,1) ne "/") { # Überprüfen, ob Befehl oder Nachricht
  &post($room,"<b><font color=$col>$alias: <font color=$secol>".$msg."</font></font></b>");
  &reload_html($whisper,$tmp);
 } else {
  if ($tmp eq "noaway") {
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat sich zurück gemeldet ... </font></i>");
   &post_prv($alias,"<script language=\"JavaScript\">open(\"main.pl?var=input_html&alias=$alias&col=$col&secol=$secol&room=$room&whisper=$whisper&tmpid=$tmpid\",\"input\");</script>");
  }
   &cmd($msg);
 }
}

sub cmd { # Alle Chatbefehle, die über input_html eingegeben werden können.
 $_[0] = $_[0]." ";
 $_[0] = substr($_[0], 1, length($_[0])-1);
 my @cmd = split(/ /, $_[0]);
 my $cmd = substr($_[0],0 , index($_[0], " "));
 my ($action);
#############>- NORMALE CHATBEFEHLE ( Alphabetisch sortiert ) -<
#############>- INFORMATIONEN ÜBER DAS SYSTEM AUSGEBEN -<
 if ($cmd eq "about") {
  &post_prv("$alias","<font size=2 color=ffffff>Aktuelle Version ist $title und wurde am $datum aktuallisiert. Created by  Paul C. Bütow 2k.</font>");
#############>- ABMELDEN MIT 'AWAY-NACHRICHT' -<
 } elsif ($cmd eq "away") {
  if ($tmp eq "away") { # Falls 2 mal hintereinander /away eingegeben wurde
   my $away_msg = "";
  }
  for ($i=1; $i <= $#cmd; $i++) {
   $away_msg = $away_msg." ".$cmd[$i];
  }
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias meldet sich ab (<font color=red>$away_msg</font> ) ...</i></font>");
  &reload_html($whisper,"away");
  exit;
#############>- EIGENEN CHATBILDSCHIRM LÖSCHEN -<
 } elsif ($cmd eq "clear") {
  &post_prv("$alias","<script language=\"JavaScript\">window.open(\"push.pl?alias=$alias&room=$room&var=clear_screen&tmpid=$tmpid\",\"main\");</script>");
  &reload_html($whisper);
  exit;
#############>- FARBEN WECHSELN -<
 } elsif ($cmd eq "col") {
  $col = $cmd[1];
  my $tmpsecol = $cmd[2];
  if ($tmpsecol eq "") {
   $secol = $col
  } else {
   $secol = $tmpsecol;
  }
  &read_file("data/user/options/$alias");
   my @pro = <DATEI>;
  close DATEI;
  &write_file_new("data/user/options/$alias");
   print DATEI $pro[0]."".$col."\n".$secol;
  close DATEI;
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=$col>$alias</font> <font color=$secol> hat die  Farbe gewechselt ...</font></i>");
  &reload_html($whisper,"change_colors");
  exit;
#############>- AKTUELLES DATUM UND URZEIT AUSGEBEN -<
 } elsif ($cmd eq "date") {
  &zeit;
  &post_prv("$alias","<font size=2 color=ffffff>Heute ist der $day.$month. Jahr $year. Es ist genau $hours:$min Uhr und $sec Sekunden.</font>");
  &reload_html($whisper);
  exit;
#############>- EINEN NEUEN RAUM ERSTELLEN BZW. IN EINEN BESTEHENDEN RAUM WECHSELN -<
 } elsif ($cmd eq "j") {
  &change_room("j",$cmd[1],$room);
  &reload_html($whisper,"change_room");
  exit;
#############>- HANDELN -<
 } elsif ($cmd eq "me" || $cmd eq "m") {
  for ($i=1; $i <= $#cmd; $i++) {
   $me_msg = $me_msg." ".$cmd[$i];
  }
  if ($cmd eq "m") {
   &post($room,"<i><font color=$col>$alias <font color=$secol>$me_msg</font></font></i>");
  } else {
   &post($room,"<i><font color=$col>$alias $me_msg</font></i>");
  }
  &reload_html($whisper);
  exit;
#############>- MEMO SCHREIBEN -<
 } elsif ($cmd eq "memo") {
  my $alias2memo = $cmd[1];
  &list_dir("data/user/options");
  foreach(@dir) {
   if(-f "data/user/options/$alias2memo") {
    for ($i=2; $i <= $#cmd; $i++) {
     $memo_msg = $memo_msg." ".$cmd[$i];
    }
    &post_prv("$alias","<font size=2 color=ffa500>$alias2memo <font color=ffffff>wurde folgende memo erfolgreich hinterlassen: $memo_msg</font></font>");
    &zeit;
    &write_file("data/user/memos/$alias2memo");
     print DATEI "<font size=2 color=ffffff>(<font color=red>$day.$month. - $hours:$min:$sec</font>) $alias hat Dir eine Memo hinterlassen: <font color=ffa500>$memo_msg</font></font><br>";
    close DATEI;
    &reload_html($whisper);
    exit;
   }
  }
  &post_prv("$alias","<font size=2 color=ffa500>$alias2memo <font color=ffffff>existiert in der Datenbank nicht !</font></font>");
  &reload_html($whisper);
  exit;
#############>- FLÜSTERN ( Auch mit '/' ) -<
 } elsif ($cmd eq "msg" || $cmd eq "") {
  my ($alias2post,$msg2post) = ($cmd[1],"");
   if ($cmd eq "msg") {
    for ($i=2; $i <= $#cmd; $i++) {
     $msg2post = $msg2post." ".$cmd[$i];
    }
   $action = "whisper";
   } else {
    $alias2post = $whisper;
    for ($i=1; $i <= $#cmd; $i++) {
     $msg2post = $msg2post." ".$cmd[$i];
    }
   }
   &list_dir("data/online/users");
   foreach(@dir) {
    if ($_ eq $alias2post) {
     &write_file("data/msgs/$alias2post");
      print DATEI "<i><font color=$col>$alias flüstert <font color=ffffff>=></font><font color=$secol>$msg2post</font></font></i><br>\n";
     close DATEI;
     &write_file("data/msgs/$alias");
      print DATEI "<i><font color=$col>Zu $alias2post geflüstert <font color=ffffff>=></font><font color=$secol>$msg2post</font></font></i><br>\n";
     close DATEI;
     &reload_html($alias2post,$action);
     exit;
    }
   }
   &post_prv("$alias","<font size=2 color=ffa500>$alias2post <font color=ffffff>ist momentan nicht anwesend !</font></font>");
#############>- TOTALONLINEZEIT VON ALIAS ANZEIGEN -<
 } elsif ($cmd eq "online") {
  my $alias2prove= $cmd[1];
  if (-f "data/user/counts/$alias2prove.online") {
   &read_file("data/user/counts/$alias2prove.online");
    my $online = <DATEI>;
   close DATEI;
    my $totaltime = sprintf("%.2f",($online / 60));
   &post_prv("$alias","<font size=2 color=ffa500>$alias2prove <font color=ffffff> war insgesamt schon $totaltime Minuten online !</font></font>");
  } else {
   &post_prv("$alias","<font size=2 color=ffa500>$alias2prove <font color=ffffff>hat z.Z. keine Statistiken !</font></font>");
  }
  &reload_html($whisper);
  exit;
#############>- DAS SYSTEM VERLASSEN -<
 } elsif ($cmd eq "q") {
  &reload_html($whisper,"quit");
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias verläßt den Chat ... </font></i>");
  sleep 2;
  &rm_alias($alias,$room);
  exit;
#############>- DAS EIGENE GÄSTEBUCH LÖSCHEN -<
 } elsif ($cmd eq "rmgb") {
  unlink("data/user/gbooks/$alias");
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias löscht sein gesamtes Gästebuch  ...</font></i>",$_);
  &reload_html($whisper);
  exit;
#############>- SCHREIEN -<
 } elsif ($cmd eq "s") {
   my ($msg2scream,$msg2post);
   for ($i=1; $i <= $#cmd; $i++) {
    $msg2scream = $msg2scream." ".$cmd[$i];
   }
  $msg2post = "\U$msg2scream";
  &post($room,"<font color=$col><i>$alias schreit:</i> <b><font color=$secol>".$msg2post."</font></font></b>");
  &reload_html($whisper);
  exit;
#############>- SEHEN, WANN SICH ALIAS ZUM LETZTEN MAL EINGELOGT HAT -<
 } elsif ($cmd eq "seen") {
  my $alias2prove = $cmd[1];
  &list_dir("data/user/options");
  foreach(@dir) {
   if (-f "data/user/lastlogin/$alias2prove") {
    &read_file("data/user/lastlogin/$alias2prove");
     my $lastlogin = <DATEI>;
    close DATEI;
     my $timestamp = sprintf("%.2f",((time() - $lastlogin) / 3600));;
    &post_prv("$alias","<font size=2 color=ffa500>$alias2prove <font color=ffffff> hat sich vor $timestamp Stunden zuletzt in das System eingelogt !</font></font>");
    &reload_html($whisper);
    exit;
   }
  }
  &post_prv("$alias","<font size=2 color=ffa500>$alias2prove <font color=ffffff> hat z.Z. keine Statistiken !</font></font>");
  &reload_html($whisper);
  exit;
#############>- ANZEIGEN WER SICH IM AKTUELLEN RAUM BEFINDET -<
 } elsif ($cmd eq "w") {
  &list_dir("data/online/rooms/$room");
  &read_file("data/online/rstat/$room");
   my @r_stat = <DATEI>;
  close DATEI;
  my $door = "[<font color=red>Abgesperrt!</font>]" if ($r_stat[0] eq "1\n");
  my ($is,$nr) = ("t",0);
  foreach(@dir) {
   if (-f "data/online/rooms/$room/$_") {
    $personen = $personen." ".$_;
    $nr++;
   }
  }
  &post_prv($alias,"<font size=2 color=ffffff>$room $door ($nr) [".$r_stat[1]."] : <font color=ffa500>$personen</font></font>");
  &reload_html($whisper);
  exit;
  #############>- ANZEIGEN WER SICH IM SYSTEM BEFINDET -<
 } elsif ($cmd eq "wc" || $cmd eq "wc/") {
  $raum = $cmd[1];
  if ($raum eq "" && $cmd eq "wc") {
   &list_dir("data/online/users");
   foreach(@dir) {
    $personen = $personen." ".$_ if (-f "data/online/users/$_");
   }
   my $nr = ($#dir-1);
   my ($nrs,$is) = ("","ist");
   if ($nr != 1) {
    $nrs = "en";
    $is = "sind";
   }
   &post_prv("$alias","<font size=2 color=ffffff>$nr Person$nrs $is insgesamt eingelogt: <font color=ffa500>$personen</font></font>");
  } elsif ($raum eq "" && $cmd eq "wc/") {
   my $msg2post;
   &list_dir("data/online/rooms");
   foreach(@dir) {
    if (-e "data/online/rooms/$_" && $_ ne ".." && $_ ne ".") {
     &list_dir("data/online/rooms/$_",2);
     &read_file("data/online/rstat/$_");
      my @rstat = <DATEI>;
     close DATEI;
     my $door = "[<font color=red>Abgesperrt!</font>]" if ($rstat[0] eq "1\n");
     my $nr = $#dir2-1;
     $msg2post = $msg2post."<tr><td><b><font size=2 color=ffffff>$_ $door ($nr) [".$rstat[1]."] </font></b></td><td><font size=2 color=ffffff>";
     foreach $chatter (@dir2) {
      if (-f "data/online/rooms/$_/$chatter") {
       $msg2post = $msg2post."$chatter ";
      }
     }
     print "</font></td>";
    }
   }
   &post_prv("$alias","<font size=2 color=ffffff>Folgende Räume sind offen:</font><table>$msg2post</table>");
  } elsif ($raum ne "") {
   my $personen;
   &list_dir("data/online/rooms/$raum");
   foreach(@dir) {
    $personen = $personen." ".$_ if (-f "data/online/rooms/$raum/$_");
   }
   my ($nr,$is) = (($#dir-1),"t");
   if ($nr != 1) {
    $nrs = "en";
    $is = "n";
   }
   &post_prv("$alias","<font size=2 color=ffffff> $nr Person$nrs befinde$is sich im Raum \"$raum\": <font color=ffa500>$personen</font></font>");
  }
  &reload_html($whisper);
  exit;
#############>- ANZEIGEN IN WELCHEN RAUM SICH JMD. BEFINDET BZW. JMD IN EINEN RAUM FOLGEN -<
 } elsif ($cmd eq "wj" || $cmd eq "ju") {
  my $alias2search = $cmd[1];
  &list_dir("data/online/rooms");
  foreach $raum (@dir) {
   &list_dir("data/online/rooms/$raum",2) if (-d "data/online/rooms/$raum");
   foreach $user (@dir2) {
    if (-f "data/online/rooms/$raum/$user") {
     if ($user eq $alias2search) {
      if ($cmd eq "wj") {
       &post_prv("$alias","<font size=2 color=ffa500>$alias2search <font color=ffffff>befindet sich im Raum \"$raum\" !</font></font>");
      } else {
       &change_room("ju",$raum,$room,$alias2search);
      }
      &reload_html($whisper,"change_room");
      exit;
     }
    }
   }
  }
  &post_prv("$alias","<font size=2 color=ffa500>$alias2search <font color=ffffff> ist nicht eingelogt !</font></font>");
  &reload_html($whisper);
  exit;
#############>- RAUMNESETZER BEFEHLE ( Alphabetisch sortiert ) -<




#############>- RAUM ABSCHLIESSEN/AUFSCHLIESSEN -<
 } elsif ($cmd eq "l") {
  &prove_besetzer($alias,$room);
  &hirachie($alias);
  if ($r_besetzer eq "true" || $hirachie eq "wa" || $hirachie eq "ow") {
   my $lock = "0\n";
   &zeit;
   if ($room eq "Cyberbar") {
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias sperrt den Raum ohne Erfolg ab ...</font></i>");
   } else {
    if ($r_stat[0] == 0) {
     $lock = "1\n";
     &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias sperrt den Raum ab ...</font></i>");
    } else {
     &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias sperrt den Raum auf ...</font></i>");
    }
    for ($i=1;$i<=$#r_stat;$i++) {
     $lock = $lock.$r_stat[$i];
    }
    write_file_new("data/online/rstat/$room");
     print DATEI $lock;
    close DATEI;
   }
  }
  &reload_html($whisper);
  exit;
#############>- RAUMBESETZER ERNENNEN -<
 } elsif ($cmd eq "su") {
  &prove_besetzer($alias,$room);
  &hirachie($alias);
  if ($r_besetzer eq "true" || $hirachie eq "wa" || $hirachie eq "ow") {
   my $alias2su = $cmd[1];
   &list_dir("data/online/rooms/$room");
   foreach(@dir) {
    if ($_ eq $alias2su) {
     &read_file("data/online/rstat/$room");
      my @rstat = <DATEI>;
     close DATEI;
     &hirachie($alias2su);
     &prove_besetzer($alias2su,$room);
     if ($r_besetzer eq "true" || $hirachie eq "wa" || $hirachie eq "ow") {
      &reload_html($whisper);
      &post_prv($alias,"<font color=ffa500>$alias2su</font> <font color=ffffff> kann nicht zum Raumbesetzer ernannt werden. (Ist bereits RB, WÄ oder OW)</font>");
      exit;
     }
     &zeit;
     &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias ernennt \"$alias2su\" zum Raumbesetzer ...</font></i>");
     &write_file("data/online/rstat/$room");
      print DATEI $alias2su."\n";
     close DATEI;
     &reload_html($whisper);
     exit;
    }
   }
   &post_prv($alias,"<font color=ffa500>$alias2su</font> <font color=ffffff> kann nicht zum Raumbesetzer ernannt werden. (Ist nicht anwesend)</font>");
  }
  &reload_html($whisper);
  exit;
#############>- RAUMBESETZER ENTNENNEN -<
 } elsif ($cmd eq "rsu") {
  &prove_besetzer($alias,$room);
  &hirachie($alias);
  if ($r_besetzer eq "true" || $hirachie eq "wa" || $hirachie eq "ow") {
   my $alias2rsu = $cmd[1];
   &read_file("data/online/rstat/$room");
    my @rstat = <DATEI>;
   close DATEI;
   &prove_besetzer($alias2rsu,$room);
   if ($r_besetzer eq "true") {
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias entzieht \"$alias2rsu\" die Raumbesetzerrechte ...</font></i>");
    my $newrstat = $rstat[0].$rstat[1];
    for ($i=2;$i<=$#r_stat;$i++) {
     $newrstat = $newrstat.$r_stat[$i] if ($r_stat[$i] ne $alias2rsu."\n");
    }
    &write_file_new("data/online/rstat/$room");
     print DATEI $newrstat;
    close DATEI;
   } else {
    &post_prv($alias,"<font size=2 color=ffa500>$alias2rsu <font color=ffffff>ist kein Raumbesetzer und kann dieser Status auch nicht aberkannt werden !</font></font>");
   }
  }
  &reload_html($whisper);
  exit;
#############>- RAUM EIN THEMA GEBEN -<
 } elsif ($cmd eq "t") {
  &prove_besetzer($alias,$room);
  &hirachie($alias);
  if ($r_besetzer eq "true" || $hirachie eq "wa" || $hirachie eq "ow") {
  my $theme;
  &zeit;
   if ($cmd[1] ne undef) {
    for ($i=1; $i <= $#cmd; $i++) {
     $theme = $theme." ".$cmd[$i];
    }
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias gibt dem Raum das Thema <font color=red>( $theme )</font> ...</font></i>");
   } else {
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias löscht das Thema des Raums ...</font></i>");
   }
  $theme = $r_stat[0].$theme."\n";
  for ($i=2;$i<=$#r_stat;$i++) {
   $theme = $theme.$r_stat[$i];
  }
  write_file_new("data/online/rstat/$room");
   print DATEI $theme;
  close DATEI;
  }
  &reload_html($whisper);
  exit;
#############>- SUPERUSER BEFEHLE ( Alphabetisch sortiert ) -<
#############>- ALIAS VERBANNEN BZW. ENTBANNEN -<
 } elsif ($cmd eq "ban" || $cmd eq "rban") {
  &hirachie($alias);
  if ($hirachie eq "ow" || $hirachie eq "wa") {
   my ($alias2handle,$registriert) = $cmd[1];
   &list_dir("data/user/options");
   foreach(@dir) {
    $registriert = "true" if ($_ eq $alias2handle);
   }
   if ($registriert ne "true") {
    if ($cmd eq "ban") {
     if ($alias2handle ne "") {
      &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> existiert nicht in der Benutzerdatenbank und kann so nich verbannt werden !</font></font>");
     } else {
      &read_file("data/ban");
       my @banned = <DATEI>;
       my ($benutzer);
       my $nr = $#banned+1;
      close DATEI;
      foreach(@banned) {
       $benutzer = $benutzer." ".$_;
      }
      &post_prv("$alias","<font size=2 color=ffffff>Folgende Benutzer sind verbannt [$nr]: <font color=ffa500>$benutzer</font></font>");
     }
    } else {
     &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> existiert nicht in der Benutzerdatenbank und kann so nicht \"entbannt\" werden !</font></font>");
    }
    &reload_html($whisper);
    exit;
   }
   &read_file("data/ban");
    my @banlist = <DATEI>;
   close DATEI;
   foreach(@banlist) {
    if ($_ eq "$alias2handle\n") {
     if ($cmd eq "ban") {
      &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> ist bereits verbannt worden !</font></font>");
     } else {
      my $banned = "false";
      foreach(@banlist) {
       $banned = "true" if ($_ eq "$alias2handle\n");
      }
      if ($banned eq "true") {
       my @newbanlist;
       foreach(@banlist) {
        push @newbanlist,$_ if ($_ ne "$alias2handle\n");
       }
       &write_file_new("data/ban");
        print DATEI @newbanlist;
       close DATEI;
       &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> wurde soeben \"entbannt\" !</font></font>");
      }
     }
     &reload_html($whisper);
     exit;
    }
   }
   if ($cmd eq "ban") {
    &hirachie($alias2handle);
    if ($hirachie ne "ow" || $hirachie eq "wa") {
     &write_file("data/ban");
      print DATEI "$alias2handle\n";
     close DATEI;
     &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> wurde soeben verbannt und kann sich nun nichtmehr in den Chat einloggen !</font></font>");
    } else {
     &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> ist ein Oberwächter und kann deshalb nicht verbannt werden !</font></font>");
    }
   } else {
    &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> konnte nicht \"entbannt\" werden, da <font color=ffa500>$alias2handle</font> garnicht verbannt war !</font></font>");
   }
  }
  &reload_html($whisper);
  exit;
#############>- IP # VON ALIAS ANZEIGEN LASSEN -<
 } elsif ($cmd eq "ip") {
  &hirachie($alias);
  if ($hirachie eq "ow" || $hirachie eq "wa") {
   my $alias2prove = $cmd[1];
   &list_dir("data/online/ident");
   foreach(@dir) {
    if ($_ eq $alias2prove) {
     &read_file("data/online/ident/$_");
      local $ipnr = <DATEI>;
     close DATEI;
     &post_prv("$alias","<font size=2 color=ffa500>$_ <font color=ffffff>hat die IP-Nummer $ipnr !</font></font>");
    }
   }
  }
  &reload_html($whisper);
  exit;
#############>- ALIAS AUS DEM CHAT KATAPULTIEREN -<
 } elsif ($cmd eq "k") {
  &hirachie($alias);
  if ($hirachie eq "wa" || $hirachie eq "ow") {
   my $alias2kick = $cmd[1];
   &list_dir("data/online/rooms");
   foreach $raum (@dir) {
    &list_dir("data/online/rooms/$raum",2);
    foreach $chatter (@dir2) {
     if ($chatter eq $alias2kick) {
      &hirachie($alias2kick);
      if ($hirachie eq "ow") {
       &post_prv("$alias","<font size=2 color=ffa500>$alias2kick <font color=ffffff> ist Oberwächter und kann nicht \"gekickt\" werden !</font></font>");
      } elsif ($hirachie eq "wa") {
       &post_prv("$alias","<font size=2 color=ffa500>$alias2kick <font color=ffffff> ist Wächter und kann nicht \"gekickt\" werden !</font></font>");
      } else {
       &post_prv("$alias2kick","<script language=\"JavaScript\">window.open(\"index.pl?tmp=kick\",\"_top\");</script>");
       &zeit;
       $_ = undef if ($room eq $raum);
       &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias katapultiert \"$alias2kick\" aus dem Chat ...</font></i>",$raum);
       &rm_alias($alias2kick,$raum);
       &check_online;
      }
      &reload_html($whisper);
      exit;
     }
    }
   }
   &post_prv("$alias","<font size=2 color=ffa500>$alias2kick <font color=ffffff> ist nicht anwesend und kann nicht \"gekickt\" werden !</font></font>");
  }
  &reload_html($whisper);
  exit;
#############>- EINEN BENUTZER AUS DEM SYSTEM LÖSCHEN -<
 } elsif ($cmd eq "kill") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   my $alias2kill = $cmd[1];
   &hirachie($alias2kill);
   if ($hirachie eq "ow") {
    &post_prv($alias,"<font size color=ffa500>$alias2kill</font> <font color=ffffff>ist Oberwächter und kann nicht aus dem System entfernt werden !</font>");
   } elsif ($hirachie eq "wa") {
    &post_prv($alias,"<font size color=ffa500>$alias2kill</font> <font color=ffffff>ist Wächter und kann nicht aus dem System entfernt werden !</font>");
   } else {
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias killt \"$alias2kill\" ganz und gar aus dem System ...</font></i>");
    unlink("data/user/options/$alias2kill");
    unlink("data/user/profiles/$alias2kill");
    unlink("data/user/gbooks/$alias2kill");
    unlink("data/user/counts/$alias2kill");
    unlink("data/user/counts/$alias2kill.online");
    unlink("data/user/lastlogin/$alias2kill");
    unlink("data/user/memos/$alias2kill");
   }
  }
  &reload_html($whisper);
  exit;
 #############>- SYSTEMNACHRICHT IN ALLE BESTEHENDEN RÄUME POSTEN -<
 } elsif ($cmd eq "sys") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   for ($i=1; $i <= $#cmd; $i++) {
    $sys_msg = $sys_msg." ".$cmd[$i];
   }
   &zeit;
   &post_all("<i><font color=ffffff>($hours:$min:$sec)</font></i> <b><font size=2 color=red>SYSTEMNACHRICHT</font></b><i><font color=ffffff>: $sys_msg ...</font></i>");
   }
  &reload_html($whisper);
  exit;
#############>- ALIAS ZUM WÄCHTER ERNNENNEN  -<
 } elsif ($cmd eq "wa") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   my $alias2wa = $cmd[1];
   &hirachie($alias2wa);
   if ($hirachie eq "wa") {
    &post_prv("$alias","<font size=2 color=ffa500>$alias2wa <font color=ffffff>ist bereits als Wächter eingetragen. Eine erneuerte Eintragung ist nicht möglich!</font></font>");
    &reload_html($whisper);
    exit;
   } elsif ($hirachie eq "ow") {
    &post_prv("$alias","<font size=2 color=ffa500>$alias2wa <font color=ffffff>ist bereits als Oberwächter eingetragen. Oberwächter können nicht gleichzeitig Wächter sein!</font></font>");
    &reload_html($whisper);
    exit;
   }
   if (-f "data/user/options/$alias2wa") {
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font><font color=ffa500> $alias ernennt \"$alias2wa\" zum Wächter ...</font></i>");
    open(WA,">>data/wa");
     print WA $alias2wa."\n";
    close WA;
    &reload_html($whisper);
    exit;
   }
   &post_prv($alias,"<font size=2 color=ffa500>$alias2wa <font color=ffffff>kann nicht zum Wächter ernannt werden da Benutzername nicht existiert!</font></font>");
   &reload_html($whisper);
   exit;
  }
#############>- ALIAS DIE WÄCHTERRECHTE ENTZIEHEN  -<
 } elsif ($cmd eq "rwa") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   my $alias2rwa = $cmd[1];
   &hirachie($alias2rwa);
   if ($hirachie eq "wa") {
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat \"$alias2rwa\" vom Wächter zum normalen Chatter degradiert ...</font></i>");
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
   } else {
    &post_prv($alias,"<font size=2 color=ffa500>$alias2rwa <font color=ffffff>ist kein Wächter und können die Wächterrechte nicht entzogen werden!</font></font>");
   }
   &reload_html($whisper);
   exit;
 }
#############>- ADMINISTRATORBEFEHLE -<
#############>- ALIAS ZUM OBERWÄCHTER ERNENNEN  -<
 } elsif ($cmd eq "ow" && $alias eq "Snooper") {
  my $alias2ow = $cmd[1];
  &hirachie($alias2ow);
  if ($hirachie eq "wa") {
   &post_prv("$alias","<font size=2 color=ffa500>$alias2ow <font color=ffffff>ist bereits als Wächter eingetragen. Wächter können nicht gleichzeitig Oberwächter sein!</font></font>");
   &reload_html($whisper);
   exit;
  } elsif ($hirachie eq "ow") {
   &post_prv("$alias","<font size=2 color=ffa500>$alias2ow <font color=ffffff>ist bereits als Oberwächter eingetragen!</font></font>");
   &reload_html($whisper);
   exit;
  }
  if (-f "data/user/options/$alias2ow") {
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font><font color=ffa500> $alias ernennt \"$alias2ow\" zum Oberwächter ...</font></i>");
   open(OW,">>data/ow");
    print OW $alias2ow."\n";
   close OW;
   &reload_html($whisper);
   exit;
  }
  &post_prv($alias,"<font size=2 color=ffa500>$alias2ow <font color=ffffff>kann nicht zum Oberwächter ernannt werden da Benutzername nicht existiert!</font></font>");
  &reload_html($whisper);
  exit;
#############>- ALIAS DIE OBERWÄCHTERRECHTE ENTZIEHEN  -<
 } elsif ($cmd eq "row" && $alias eq "Snooper") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   my $alias2row = $cmd[1];
   &hirachie($alias2row);
   if ($hirachie eq "ow") {
    &zeit;
    &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat \"$alias2row\" vom Oberwächter zum normalen Chatter degradiert ...</font></i>");
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
   } else {
    &post_prv($alias,"<font size=2 color=ffa500>$alias2row <font color=ffffff>ist kein Oberwächter und können die Wächterrechte nicht entzogen werden!</font></font>");
   }
   &reload_html($whisper);
   exit;
 }
#############>- PASSWORD EINES BENUTZERS ANZEIGEN LASSEN -<
 } elsif ($cmd eq "pw" && $alias eq "Snooper") {
   my $pw2show = $cmd[1];
   if (-f "data/user/options/$pw2show") {
    open(PW,"<data/user/options/$pw2show");
     @pw = <PW>;
    close PW;
   &post_prv($alias,"<font size=2 color=ffa500>$pw2show <font color=ffffff> hat das Passwort \"".$pw[0]."\" !</font></font>");
   &reload_html($whisper);
   exit;
   }
  &post_prv($alias,"<font size=2 color=ffa500>$pw2show <font color=ffffff> existiert nicht in der Benutzerdatenbank !</font></font>");
  &reload_html($whisper);
  exit;
#############>- SYSTEMMELDUNG POSTEN  -<
 } elsif ($cmd eq "cool" && $alias eq "Snooper") {
    for ($i=1; $i <= $#cmd; $i++) {
     $msg2post = $msg2post." ".$cmd[$i];
    }
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$msg2post</font></i>");
   &reload_html($whisper);
   exit;
#############>- ENDE DER CHATBEFEHLE -<
}
&reload_html($whisper,$action);
}

sub change_room { # Den Chat-Raum wechseln
 my ($method,$newroom,$oldroom,$alias2follow) = @_;
 open(RSTAT, "<data/online/rstat/$newroom"); #Überprüfen, ob Alias Raum betreten darf
  my @rstat = <RSTAT>;
 close RSTAT;
 if ($rstat[0] eq "1\n") {
  &hirachie($alias);
  if ($hirachie eq "wa" || $hirachie eq "ow") { } else {
   &post_prv("$alias","<font size=2 color=ffffff>Der Raum \"<font color=ffa500>$newroom</font>\" ist leider abgesperrt!</font>");
   &reload_html($whisper,$action);
   exit;
  }
 }
 $room = $oldroom;
 &zeit;
 if ($method eq "j") {
 unlink("data/online/rooms/$oldroom/$alias");
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias wechselt in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias kommt aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
  &post_prv("$alias","<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>Du wechselst aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
 } elsif ($method eq "ju") {
 unlink("data/online/rooms/$oldroom/$alias");
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias folgt \"$alias2follow\" in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias kommt aus dem Raum \"$oldroom\" und folgt \"$alias2follow\" herein nach \"$newroom\" ...</font></i>");
  &post_prv("$alias","<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>Du folgst \"$alias2follow\" aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
 }
 unlink("data/online/rooms/$oldroom/$alias");
 &rm_rstat($alias,$oldroom);
 &check_online;
}

sub prove_besetzer { #  Prüfen, ob Benutzer Raumbesetzerrechte hat
 my ($r_alias,$r_room) = @_;
 $r_besetzer = "false";
 &read_file("data/online/rstat/$r_room");
  @r_stat = <DATEI>;
 close DATEI;
  for($i=2;$i<=$#r_stat;$i++) {
  $r_besetzer = "true" if ($r_stat[$i] eq $r_alias || $r_stat[$i] eq $r_alias."\n");
 }
}