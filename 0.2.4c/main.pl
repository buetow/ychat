#!/usr/bin/perl -w

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen

#####################################
##WELCHE SUB MUß AUSGEFÜHRT WERDEN?##
#####################################
if ($var eq "input_html") {
 &input_html;
} elsif ($var eq "post_msg") {
 &post_msg;
} elsif ($var eq "change_room") {
 &change_room;
} else {
 &error("Parameter fehlt");
}

sub input_html { # Eingabemaske für den Chat.
 my ($reload,$action,$whisper,$lastmsg) = @_;
 $action = $q->param('action') if ($action eq "");
 if ($tmp eq "away") {
  &zeit;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias meldet sich zurück ... </font></i>");
 } elsif ($tmp eq "login") {
  &remove_rooms;
  &zeit;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat den Chat betreten ...</font></i>");
  &log("logins","\"login_ok\"");
 }
 if ($action ne "" && $action ne "whisper" && $action ne "away") {
  print "<html><head><script language=\"JavaScript\">";
  if ($action eq "change_room") {
   print "open(\"online.pl?alias=$alias&room=$room\",\"online\");";
   print "open(\"main.pl?var=input_html&alias=$alias&room=$room&col=$col&secol=$secol\",\"input\");";
   print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol\",\"menu\");";
  } elsif ($action eq "change_colors") {
   print "open(\"htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol\",\"menu\");";
  }
  print "</script></head><body background=\"$gfxpath/unten.gif\" bgcolor=ffa500>";
 } else {
  print "<html><body background=\"$gfxpath/unten.gif\" bgcolor=ffa500>";
 }
 print "<form action=\"main.pl\">";
 print "<input type=\"hidden\" name=\"tmp\" value=\"away\">" if ($action eq "away");
 print <<END;
 <select NAME="col" size="1">
        <option selected value="$col">Nickfarbe ($col)</option>
        <option value="0000FF">Blau (1)</option>
        <option value="008000">Grün</option>
        <option value="000000">Schwarz</option>
        <option value="808000">Oliv</option>
        <option value="000080">Marineblau</option>
        <option value="800080">Purpur</option>
        <option value="808080">Braun</option>
        <option value="FF00FF">Fuchsia</option>
        <option value="008080">Stahlblau</option>
        <option value="333333">Mittelschwarz</option>
        <option value="003399">BärBlau</option>
        <option value="0066CC">Blau (2)</option>
        <option value="009933">Grün</option>
        <option value="006666">Oliv</option>
        <option value="FF0000">VollRot</option>
        <option value="CC0000">RostRot</option>
        <option value="990099">Lila</option>
        <option value="FF00FF">Pink</option>
        <option value="FF9900">Orange</option>
        <option value="CC6600">Ocker</option>
        <option value="996600">Braun</option>
        <option value="666666">Grau</option>
        <option value="0A6307">Dunkelgrün</option>
        <option value="9C1B7C">Dunkellila</option>
        <option value="3D0DB9">Dunkelblau</option>
        <option value="8FA5ED">Hellblau</option>
        <option value="FFFF00">Gelb</option>
        <option value="3D4863">?????</option>
      </select>
      <select NAME="secol" size="1">
        <option selected value="$secol">Schriftfarbe ($secol)</option>
        <option value="0000FF">Blau (1)</option>
        <option value="008000">Grün</option>
        <option value="000000">Schwarz</option>
        <option value="808000">Oliv</option>
        <option value="000080">Marineblau</option>
        <option value="800080">Purpur</option>
        <option value="808080">Braun</option>
        <option value="FF00FF">Fuchsia</option>
        <option value="008080">Stahlblau</option>
        <option value="333333">Mittelschwarz</option>
        <option value="003399">BärBlau</option>
        <option value="0066CC">Blau (2)</option>
        <option value="009933">Grün</option>
        <option value="006666">Oliv</option>
        <option value="FF0000">VollRot</option>
        <option value="CC0000">RostRot</option>
        <option value="990099">Lila</option>
        <option value="FF00FF">Pink</option>
        <option value="FF9900">Orange</option>
        <option value="CC6600">Ocker</option>
        <option value="996600">Braun</option>
        <option value="666666">Grau</option>
        <option value="0A6307">Dunkelgrün</option>
        <option value="9C1B7C">Dunkellila</option>
        <option value="3D0DB9">Dunkelblau</option>
        <option value="8FA5ED">Hellblau</option>
        <option value="FFFF00">Gelb</option>
        <option value="3D4863">?????</option>
      </select>
 <input type="hidden" name="var" value="post_msg">
 <input type="hidden" name="alias" value="$alias">
 <input type="hidden" name="room" value="$room">
 <input type="hidden" name="id" value="$whisper">
 <input type="hidden" name="lastmsg" value="$lastmsg">
 <br>
 <input size="80" maxlength="250" name="msg">
 <input type="submit" value="Texten">

</form>
END
if ($reload eq "true") {
print <<END;
 <script language="JavaScript">
  <!--
   document.forms[0].msg.focus();
  //-->
 </script>
END
}
print
 $q->end_html;
}

sub post_msg { # Wird aufgerufen, wenn aus input_html eine Eingabe gepostet wurde.
 $| = 1; # Cache von Perl ausschalten
 $msg =~ s#<#&lt\;#g;		# HTML-Tags deaktivieren
 $msg =~ s#kilahu#yChat#g;
 $msg =~ s#ooperle#per!#g;
 if (substr($msg,0,1) ne "/") { # Überprüfen, ob Befehl oder Nachricht
  $msg = "<b><font color=$col>$alias: <font color=$secol>".$msg."</font></font></b>";
  $msg =~ s#"#'#g;		# " in ' umtauschen für den befehl "//"
  &input_html("true","",$id,$msg); # -> Reload=true, Action=none, Whisper=$id
  &secure_checkip($alias,1);
  &post($msg);
 } else {
  &secure_checkip($alias,2);
  &cmd($msg);
 }
}

sub cmd { # Alle Chatbefehle, die über input_html eingegeben werden können.
 $_[0] = $_[0]." ";
 $_[0] = substr($_[0], 1, length($_[0])-1);
 my @cmd = split(/ /, $_[0]);
 my $cmd = substr($_[0],0 , index($_[0], " "));
 my ($reload,$action,$whisper) = ("true","",$id);
#############>- NORMALE CHATBEFEHLE ( Alphabetisch sortiert ) -<
#############>- DER LETZTEN ZUGEFLÜSTERTEN PERSON NOCHEINMAL FLÜSTERN -<
  if ($cmd eq "/") {
   &post($q->param('lastmsg'));
#############>- INFORMATIONEN ÜBER DAS SYSTEM AUSGEBEN -<
 } elsif ($cmd eq "about") {
  &post_prv("$alias","<font size=2 color=ffffff>Aktuelle Version ist $title und wurde am $datum aktuallisiert. Created by  Paul C. Bütow 2k.</font>");
#############>- ABMELDEN MIT 'AWAY-NACHRICHT' -<
 } elsif ($cmd eq "away") {
  if ($tmp eq "away") { # Falls 2 mal hintereinander /away eingegeben wurde
   $tmp = "";
  }
  for ($i=1; $i <= $#cmd; $i++) {
   $tmp = $tmp." ".$cmd[$i];
  }
  &zeit;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias meldet sich ab (<font color=red>$tmp</font> ) ...</i></font>");
  $action = "away";
#############>- EIGENEN CHATBILDSCHIRM LÖSCHEN -<
 } elsif ($cmd eq "clear") {
  &post_prv("$alias","<script language=\"JavaScript\">window.open(\"push.pl?alias=$alias&room=$room&var=clear_screen\",\"main\");</script>");
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
  &log("system","\"edit_colors_ok\"");
  &zeit;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=$col>$alias</font> <font color=$secol> hat die  Farbe gewechselt ...</font></i>");
  $action = "change_colors";
#############>- AKTUELLES DATUM UND URZEIT AUSGEBEN -<
 } elsif ($cmd eq "date") {
  &zeit;
  &post_prv("$alias","<font size=2 color=ffffff>Heute ist der $day.$month. Jahr $year. Es ist genau $hours:$min Uhr und $sec Sekunden.</font>");
#############>- EINEN NEUEN RAUM ERSTELLEN BZW. IN EINEN BESTEHENDEN RAUM WECHSELN -<
 } elsif ($cmd eq "j") {
  &change_room("j",$cmd[1],$room);
  $action = "change_room";
#############>- HANDELN -<
 } elsif ($cmd eq "me" || $cmd eq "m") {
  for ($i=1; $i <= $#cmd; $i++) {
   $me_msg = $me_msg." ".$cmd[$i];
  }
  if ($cmd eq "m") {
   &post("<i><font color=$col>$alias <font color=$secol>$me_msg</font></font></i>");
  } else {
   &post("<i><font color=$col>$alias $me_msg</font></i>");
  }
#############>- FLÜSTERN ( Auch mit '/' ) -<
 } elsif ($cmd eq "msg" || $cmd eq "") {
  my ($alias2post,$msg2post) = ($cmd[1],"");
   if ($cmd eq "msg") {
    for ($i=2; $i <= $#cmd; $i++) {
     $msg2post = $msg2post." ".$cmd[$i];
    }
   } else {
    $alias2post = $id;
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
     &input_html($reload,"",$alias2post);
     exit;
    }
   }
   &post_prv("$alias","<font size=2 color=ffa500>$alias2post <font color=ffffff>ist momentan nicht anwesend !</font></font>");
#############>- DAS SYSTEM VERLASSEN -<
 } elsif ($cmd eq "q") {
  &post_prv("$alias","<script language=\"JavaScript\">window.open(\"index.pl?tmp=quit\",\"_top\");</script>");
  sleep 1;
  &remove_online($alias,$room);
  &secure_removeip($alias);
  unlink("data/msgs/$alias");
  exit;
#############>- SCHREIEN -<
 } elsif ($cmd eq "s") {
   my $msg2scream,$msg2post;
   for ($i=1; $i <= $#cmd; $i++) {
    $msg2scream = $msg2scream." ".$cmd[$i];
   }
  $msg2post = "\U$msg2scream";
  &post("<font color=$col><i>$alias schreit:</i> <b><font color=$secol>".$msg2post."</font></font></b>");
#############>- ANZEIGEN WER SICH IM AKTUELLEN RAUM BEFINDET -<
} elsif ($cmd eq "w") {
  &list_dir("data/online/rooms/$room");
  my ($is,$nr) = ("t",0);
  foreach(@dir) {
   if (-f "data/online/rooms/$room/$_") {
    $personen = $personen." ".$_;
    $nr++;
   }
  }
  if ($nr != 1) {
   $nrs = "en";
   $is = "n";
  }
  &post_prv("$alias","<font size=2 color=ffffff>$nr Person$nrs befinde$is sich im aktuellen Raum (\"$room\"): <font color=ffa500>$personen</font></font>");
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
     my $nr = $#dir2-1;
     $msg2post = $msg2post."<tr><td><b><font size=2 color=ffffff>$_ ($nr)</font></b></td><td><font size=2 color=ffffff>";
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
       $action = "change_room";
       &change_room("ju",$raum,$room,$alias2search);
      }
      &input_html($reload,$action,$whisper);
      exit;
     }
    }
   }
  }
  &post_prv("$alias","<font size=2 color=ffa500>$alias2search <font color=ffffff> ist nicht eingelogt !</font></font>");
#############>- SUPERUSER BEFEHLE ( Alphabetisch sortiert ) -<
#############>- ALIAS VERBANNEN BZW. ENTBANNEN -<
 } elsif ($cmd eq "ban" || $cmd eq "rban") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
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
    &input_html("true");
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
       &log("system","\"ow_rban_$alias2handle_ok\"");
      }
     }
     &input_html("true");
     exit;
    }
   }
   if ($cmd eq "ban") {
    &hirachie($alias2handle);
    if ($hirachie ne "ow") {
     &write_file("data/ban");
      print DATEI "$alias2handle\n";
     close DATEI;
     &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> wurde soeben verbannt und kann sich nun nichtmehr in den Chat einloggen !</font>");
     &log("system","\"ow_ban_".$alias2handle."_ok\"");
    } else {
     &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> ist ein Oberwächter und kann deshalb nicht verbannt werden !</font></font>");
     &log("system","\"ow_ban_".$alias2handle."_ow\"");
    }
   } else {
    &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff> konnte nicht \"entbannt\" werden, da <font color=ffa500>$alias2handle</font> garnicht verbannt war !</font></font>");
   }
  }
#############>- IP # VON ALIAS ANZEIGEN LASSEN -<
 } elsif ($cmd eq "ip") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
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
       &log("system","wa_k_$alias2kick_ow"); #!!
      } elsif ($hirachie eq "wa") {
       &post_prv("$alias","<font size=2 color=ffa500>$alias2kick <font color=ffffff> ist Wächter und kann nicht \"gekickt\" werden !</font></font>");
       &log("system","wa_k_$alias2kick_ow"); #!!
      } else {
       &post_prv("$alias2kick","<script language=\"JavaScript\">window.open(\"index.pl?tmp=kick\",\"_top\");</script>");
       &zeit;
       $_ = undef if ($room eq $_);
       &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias katapultiert \"$alias2kick\" aus dem Chat ...</font></i>",$_);
       &log("system","wa_k_$alias2kick");    #!!
       &remove_online($alias2kick,$_,"kick");
       &secure_removeip($alias2kick);
      }
      &input_html("true");
      exit;
     }
    }
   }
   &post_prv("$alias","<font size=2 color=ffa500>$alias2kick <font color=ffffff> ist nicht anwesend und kann nicht \"gekickt\" werden !</font></font>");
  }
#############>- SYSTEMNACHRICHT IN ALLE BESTEHENDEN RÄUME POSTEN -<
 } elsif ($cmd eq "sys") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   for ($i=1; $i <= $#cmd; $i++) {
    $sys_msg = $sys_msg." ".$cmd[$i];
   }
   &zeit;
   &post_all("<i><font color=ffffff>($hours:$min:$sec)</font></i> <b><font size=2 color=red>Systemnachricht von $alias für alle</font></b><i><font color=ffffff>: $sys_msg ...</font></i>");
   &log("system","\"sys_msg_$room\"");
  }
#############>- ALIAS ZUM WÄCHTER ERNNENNEN BZW. ZUM NORMALEN CHATTER DEGRADIEREN -<
 } elsif ($cmd eq "wa" || $cmd eq "rwa") {
  &hirachie($alias);
  if ($hirachie eq "ow") {
   &zeit;
   my $alias2handle = $cmd[1];
   if ($cmd eq "wa") {
    my $anerkennen = "false";
    &list_dir("data/user/options");
    foreach(@dir) {
     if ($_ eq $alias2handle) {
      $anerkennen = "true";
     }
    }
    if ($anerkennen eq "true") {
     &hirachie($alias2handle);
     if ($hirachie eq "wa") {
      &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff>ist bereits als Wächter eingetragen. Eine erneuerte Eintragung ist nicht möglich!</font></font>");
      &log("system","\"oh_wa_".$alias2handle."_exists\"");
     } elsif ($hirachie eq "ow") {
      &post_prv("$alias","<font size=2 color=ffa500>$alias2handle <font color=ffffff>ist schon als Oberwächter eingetragen. Oberwächter können nicht gleichzeitig Wächter sein!</font></font>");
      &log("system","\"oh_wa_".$alias2handle."_oh-exists\"");
     } else {
      &write_file("data/wa");
       print DATEI $alias2handle."\n";
      close DATEI;
      &post("<i><font color=ffffff>($hours:$min:$sec)</font><font color=ffa500> $alias ernennt \"$alias2handle\" zum Wächter ...</font></i>");
      &log("system","\"oh_wa_".$alias2handle."_ok\"");
     }
    } else {
     &post_prv($alias,"<font size=2 color=ffa500>$alias2handle <font color=ffffff>kann nicht zum Wächter ernannt werden da Benutzername nicht existiert!</font></font>");
     &log("system","\"oh_wa_".$alias2handle."_notexists\"");
    }
   } elsif  ($cmd eq "rwa") {
    &read_file("data/wa");
     my @wafile = <DATEI>;
    close DATEI;
    my $aberkennen = "false";
    foreach(@wafile) {
     if ($_ ne "$alias2handle\n") {
      push @newafile,$_;
    } else {
     $aberkennen = "true";
    }
   }
   &write_file_new("data/wa");
    print DATEI @newafile;
   close DATEI;
   if ($aberkennen eq "true") {
    &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat \"$alias2handle\" vom Wächter zum normalen Chatter degradiert ...</font></i>");
    &log("system","\"oh_rwa_".$alias2handle."_ok\"");
	   } else {
    &post("<font size=2 color=ffa500>$alias2handle <font color=ffffff>konnte der Wächterstatus nicht aberkannt werden da Benutzer kein Wächter ist!</font></font>");
    &log("system","\"oh_rwa_".$alias2handle."_notexists\"");
   }
  }
 }
#############>- ENDE DER CHATBEFEHLE -<
}
&input_html($reload,$action,$whisper);
}

sub change_room { # Den Chat-Raum wechseln
 my ($method,$newroom,$oldroom,$alias2follow) = @_;
  $room = $oldroom;
 &secure_checkip($alias);	# IP Nummer überprüfen
 &zeit;                		# Zeit 'erneuern'
 if ($method eq "j") {
  unlink("data/online/rooms/$oldroom/$alias");
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias wechselt in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias kommt aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
  &post_prv("$alias","<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>Du wechselst aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
 } elsif ($method eq "ju") {
  unlink("data/online/rooms/$oldroom/$alias");
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias folgt \"$alias2follow\" in den Raum \"$newroom\" ...</font></i>");
  $room = $newroom;
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias kommt aus dem Raum \"$oldroom\" und folgt \"$alias2follow\" herein nach \"$newroom\" ...</font></i>");
  &post_prv("$alias","<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>Du folgst \"$alias2follow\" aus dem Raum \"$oldroom\" herein nach \"$newroom\" ...</font></i>");
 }
 &rm_rooms;
}

