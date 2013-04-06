#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print $q->header();

#####################################
##WELCHE SUB MUß AUSGEFÜHRT WERDEN?##
#####################################

$var = $q->param("var");
if ($var eq "list_alias_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &list_alias_html;
} elsif ($var eq "alias_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 $tmp = $q->param("tmp");
 &alias_html;
} elsif ($var eq "edit_profile_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 $room = $q->param("room");
 &edit_html("edit_profile_html");
} elsif ($var eq "edit_options_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 $room = $q->param("room");
 $col = $q->param("col");
 $secol = $q->param("secol");
 &edit_html("edit_options_html");
} elsif ($var eq "gb_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &gb_html;
} elsif ($var eq "post_gb") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 $msg = $q->param("msg");
 $tmp = $q->param("tmp");
 $id = $q->param("id");
 &post_gb;
} elsif ($var eq "edit_profile") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 $room = $q->param("room");
 &secure_checkid($alias);
 &edit("edit_profile");
} elsif ($var eq "edit_options") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 $tmp = $q->param("tmp");
 $id = $q->param("id");
 $room = $q->param("room");
 $col = $q->param("col");
 $secol = $q->param("secol");
 &edit("edit_options");
} elsif ($var eq "rm_gb_alias") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 $tmp = $q->param("tmp");
 $id = $q->param("id");
 &rm_gb;
} else {
 &error("Parameter fehlt");
}

###########################
##HAUPTTEIL DIESER DATEI:##
###########################

sub gb_html { # Das Benutzerboard/Gästebuch.
 my $message = $_[0];
 &start_html("Benutzerboard");
if ($message eq "post_empty") {
print <<END;
 <div class=b>
  ACHTUNG!
 </div>
 <div>
  Du wolltest soeben einen leeren Eintrag posten. Bitte schreib
  <i>
   zuerst
  </i>
  einen Text und poste
  <u>dann</u>
  !
 </div>
 <hr>
END
} elsif ($message eq "post_ok") {
print <<END;
 <div class=b>
  Meldung:
 </div>
 <div>
  Vielen Dank für Deinen Beitrag. Wenn Du willst, kannst Du jetzt gleich noch einen Eintrag posten ...
 </div>
 <hr>
END
} else {
print <<END;
 <div>
  Falls Du Anregungen oder Verbesserungsvorschläge für die Community hast, dann poste doch einen kleinen Eintrag :-)
 </div>
 <hr>
END
}
print <<END;
 <form action="main2.pl">
  <input type="hidden" name="var" value="post_gb">
  <input type="hidden" name="alias" value="$alias">
  <input type="hidden" name="tmpid" value="$tmpid">
  <input type="hidden" name="tmp" value="guestbook">
  <input type="hidden" name="id" value="y">
  <input size="40" name="msg">
  <font color="ffffff">
   <input type="submit" value="Hinterlassen ...">
  </font>
 </form>
 <hr>
END
 if (-e "data/guestbook") {
  open (DATEI,"<data/guestbook") || &error("Konnte dat/guestbook nicht öffnen");
   @datei = <DATEI>;
  close DATEI;
  print
   $q->div(@datei);
 }
 print
  $q->end_html;
}


sub post_gb { # Wenn etwas in ein Gästebuch eingetragen wird
 &zeit;
 if ($msg eq "") {
  if ($id eq "y") { # Überprüfen, ob Benutzerboard- oder Privatgästebuch
   &gb_html("post_empty");
  } else {
   &alias_html("post_empty");
  }
  exit;
 }
 $msg =~ s#<#&lt\;#g;
 $msg =~ s#\n#<br>#g;
 my $tmpath = "";
 if ($id eq "y") {
  $tmpath = "data/guestbook";
 } else {
  $tmpath = "data/user/gbooks/$tmp";
 }
 open(GB,"$tmpath");
  my @gb = <GB>;
 close GB;
 pop(@gb) if ($#gb > 99);
 my @newdatei = "<br><b>$alias schrieb am $day.$month.$year um $hours:$min:$sec ...</b><br>$msg<br>\n";
 push(@newdatei,@gb);
 open(GB,">$tmpath");
  print GB @newdatei;
 close GB;
 if ($id eq "y") {
  &gb_html("post_ok");
  exit;
 } else {
  post_prv("$tmp","<font color=ffffff>$alias hat soeben einen Pin in Dein Profil gehängt ... </font>");
  &alias_html;
  exit;
 }
}

sub rm_gb { # Gästebucheintrag entfernen
 open(GB,"<data/user/gbooks/$alias");
  my @gbfile = <GB>;
 close GB;
 my @newgbfile = ();
  for ($i=0; $i <= $#gbfile; $i++) {
   push(@newgbfile,$gbfile[$i]) if ($i != $id);
  }
  open(GB,">data/user/gbooks/$alias");
   print GB @newgbfile;
  close GB;
 &alias_html("rm_ok");
}

sub list_alias_html { # Liste aller Benutzer
 &start_html("Alle Benutzer");
 opendir(REGISTERED,"data/user/options");
  my @users = readdir(REGISTERED);
 closedir(REGISTERED);
 my @liste = sort(@users);
 my $nr = $#users-1;
print <<END;
 <div>
  Es sind z.Z. genau $nr Benutzernamen in der Datenbank vorhanden! (Sortiert nach #, A-Z, a-z)
 </div>
 <hr>
 <div>
END
 foreach(@liste) {
  if (-f "data/user/options/$_") {
   print
    $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid" },
            "$_"),
    " ";
  }
 }
print <<END;
 </div>
</body>
</html>
END
}

sub alias_html { # Anzeigen eines Benutzerprofils
 my $message = $_[0];
 open(COUNTS,"<data/user/counts/$tmp");
  local $counter = <COUNTS>;
 close COUNTS;
 $counter++;
 open(COUNTS,">data/user/counts/$tmp");
  print COUNTS $counter;
 close COUNTS;
 &start_html($tmp);
print <<END;
<div>
 <a href="htdocs.pl?var=werda_html&alias=$alias&tmpid=$tmpid">Hier</a>
 gehts zur Gesamtübersicht !
</div>
<hr>
END
if ($message eq "rm_ok") {
print <<END;
 <div class=b>
  Meldung:
 </div>
 <div>
  Der von Dir gewählte Eintrag wurde erfolgreich gelöscht.
 </div>
 <hr>
END
} elsif ($message eq "post_empty") {
print <<END;
 <div class=b>
  ACHTUNG!
 </div>
 <div>
  Du wolltest soeben einen leeren Eintrag posten. Bitte schreib
  <i>
   zuerst
  </u>
  einen Text und poste
  <u>dann</u>
  !
  <hr>
END
}
 open(PROFILE,"<data/user/profiles/$tmp");
  @data = <PROFILE>;
 close PROFILE;
print <<END;
  <div>
   Dieses Profil wurde insgesamt schon $counter mal aufgerufen !
   <br><br>Alias: $tmp
   <br><br>Nachname: $data[0]
   <br><br>Vorname: $data[1]
   <br><br>Alter: $data[2]
   <br><br>Wohnort: $data[3]
   <br><br>E-Mail:
   <a href="mailto:$data[4]">$data[4]</a>
   <br><br>ICQ-Nummer: $data[5]
   <br><br>Homepage: <a href="$data[6]" target="_new">$data[6]</a>
   <br><br>Weiteres:<br>$data[7]
   <hr>
   Hier kannst Du $tmp eine Nachricht hinterlassen!
   <form action="main2.pl">
    <input type="hidden" name="var" value="post_gb">
    <input type="hidden" name="alias" value="$alias">
    <input type="hidden" name="tmpid" value="$tmpid">
    <input type="hidden" name="tmp" value="$tmp">
    <input type="text" size="40" name="msg">
    <font color="ffffff">
     <input type="submit" value="Hinterlassen ...">
    </font>
   </form>
   <hr>
END
if (-e "data/user/gbooks/$tmp") {
  open (DATEI,"<data/user/gbooks/$tmp") || &error("Konnte data/user/gbooks/$tmp nicht öffnen");
   @datei = <DATEI>;
  close DATEI;
  my $nr = 1;
  if ($alias ne $tmp) {
   foreach(@datei) {
    print "<br>Eintrag # $nr: $_";
    $nr++;
   }
  } else {
print <<END;
   <table>
    <form action="main2.pl">
     <input type="hidden" name="var" value="rm_gb_alias">
     <input type="hidden" name="alias" value="$alias">
     <input type="hidden" name="tmpid" value="$tmpid">
     <input type="hidden" name="tmp" value="$tmp">
END
for($i=0; $i <= $#datei; $i++) {
print <<END;
     <tr>
      <td>
       <input type="radio" name="id" value="$i">
      </td>
      <td>
       <div class=q>
       <br>
       Eintrag # $nr:
        $datei[$i]
       </div>
      </td>
     </tr>
END
$nr++;
}
print <<END;
    </table>
    <table>
     <tr>
      <td>
       <font color=ffffff>
        <input type="submit" value="Markierten Eintrag löschen">
       </font>
      </td>
      <td>
       <font color=ffffff>
        <input type="reset" value="Markierung rückgängig machen">
       </font>
      </td>
     </tr>
    </table>
   </form>
END
}
}
 print
  $q->end_html;
}

sub edit_html { # Chat-Optionen / Benutzerprofil editieren
 my ($method,$message) = @_;
 &start_html("Benutzerprofil ändern");
 if ($method eq "edit_options_html") {
  open(DATEI,"<data/user/options/$alias");
 } elsif ($method eq "edit_profile_html") {
  open(DATEI,"<data/user/profiles/$alias");
 }
 @data = <DATEI>;
 close DATEI;
 foreach(@data) {
  chomp $_;
 }
 if ($method eq "edit_options_html") {
  if ($message eq "edit_options_ok") {
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=$col>$alias</font> <font color=$secol> hat die Chateinstellungen geändert ...</font></i>");
print <<END;
<script language="JavaScript">
 open("main.pl?var=change_colors&alias=$alias&room=$room&col=$col&secol=$secol&tmpid=$tmpid","reload");
</script>
<div class=b>
 Hinweis:
</div>
<div>
 Du hast soeben Deine Chat-Optionen erfolgreich editiert. Hier kannst Du Dein Profil erneuert editieren. Um
 weiterzuchatten einfach Fenster schliessen!
</div>
<hr>
END
  } elsif ($message eq "edit_pw_wrong") {
print <<END;
<div class=b>
 ACHTUNG!
</div>
<div>
 Du hast soeben versucht, dein Passwort zu ändern. Jedoch stimmte die Erste Eingabe nicht mit der Zweiten überein.
 Bitte versuch es erneuert ...
</div>
<hr>
END
  } else {
   print
    $q->div( "Hier kannst Du Deine Chat-Optionen editieren!" ),
    $q->hr;
  }
print <<END;
<script lang="javascript" type="text/javascript">
<!--

function radioWert(rObj) {
  for (var i=0; i<rObj.length; i++) if (rObj[i].checked) return rObj[i].value;
  return false;
}

function insertcode( code ) {
  document.optionen.elements[radioWert(document.optionen.eingabe)].value = code;
}

//-->
</script>

<form name="optionen" action="main2.pl">
 <input type="hidden" name="var" value="edit_options">
 <table>
  <tr>
   <td>
    <div>
     Dein Passwort
     <font color=ffa500>
      (Gut merken !!)
     </font>
     <br>
     <input type="password" name="id" value="$data[0]">
    </div>
   </td>
   <td width=20>
   </td>
   <td>
    <div>
     Geb hier das Passwort erneuert ein:
     <br>
     <input type="password" name="tmp" value="$data[0]">
    </div>
   </td>
  </tr>
 </table>
 <hr>
 <div>Einstellungen für den Nachrichten-Verlauf</div>
 <table>
  <tr>
   <td>
    <br>
    <div>Anzahl der Nachrichten im Verlauf (Standart = 10)</div>
    <font color=ffffff>
     <select name="maxVerlauf" size="1">
     <option value="$data[3]">$data[3]
END
for($i=1;$i<=50;$i++) {
 print "<option value=\"$i\">$i";
}
print<<END;
     </select>
    </font>
   </td>
  </tr>
  <tr>
   <td>
      <br>
<div>Wie sollen die Nachrichten im Verlauf angeordnet sein?</div>
    <font color=ffffff>
     <select name="verlaufOrder" size="1">
      <option value="$data[4]">Bisherige Einstellung ($data[4])
      <option value="0">Neue Verlaufnachrichten oben (0)
      <option value="1">Neue Verlaufnachrichten unten (1)
     </select>
    </font>
   </td>
  </tr>
 </table>
<br>
 <div>PS: Einstellungen des Nachrichten-Verlaufs werden erst nach einem Neueinloggen aktualisiert.</div>
 <hr>
 <table>
  <tr>
   <td>
    <div>
     <input type="radio" name="eingabe" value="col" checked>
      Auswahl von Farbe 1
      <br>
      (Farbe Deines Nicks):
      <br>
      <input name="col" size="6" maxlength="6" value="$data[1]">
     </div>
    </td>
   <td width=20>
   </td>
    <td>
    <div>
     <input type="radio" name="eingabe" value="secol">
      Auswahl von Farbe 2
      <br>
      (Farbe Deiner Nachrichten):
      <br>
      <input name="secol" size="6" maxlength="6" value="$data[2]">
     </div>
    </td>
   </tr>
  </table>
<br>
 <div>
  Beliebige Auswahl markieren und Farbe wählen. Um Deine Optionen abzuspeichern,
  auf "Daten ändern" klicken!
 </div>
 <br>
END
 open(COLORS,"<colors.sno");
  @colors = <COLORS>;
 close COLORS;
 print @colors;
 } elsif ($method eq "edit_profile_html") {
  if ($message eq "edit_profile_ok") {
   &zeit;
   &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias hat das Profil geändert ...</font></i>");
print <<END;
 <div class=b>
  Hinweis:
 </div>
 <div>
 Du hast soeben Dein Profil erfolgreich editiert. Hier kannst Du Dein Profil erneuert editieren:
 <hr>
END
  } else {
   print
    $q->div( "Hier kannst Du Dein Profil editieren!" ),
    $q->hr;
  }
print <<END;
 <form action="main2.pl">
  <input type="hidden" name="var" value="edit_profile">
  <div>
   Dein Nachname:
   <br>
   <input name="nachname" value="$data[0]">
  </div>
  <br>
  <div>
   Dein Vorname:
   <br>
   <input name="vorname" value="$data[1]">
  </div>
  <br>
  <div>
   Dein Alter:
   <br>
   <input name="alter" value="$data[2]">
  </div>
  <br>
  <div>
   Dein Wohnort:
   <br>
   <input name="wohnort" value="$data[3]">
  </div>
  <br>
  <div>
   Deine E-Mail Adresse:
   <br>
   <input name="email" value="$data[4]">
  </div>
  <br>
  <div>
   Deine ICQ-Nummer:
   <br>
   <input name="icq" value="$data[5]">
  </div>
  <br>
  <div>
   Deine Homepage:
   <br>
   <input name="homepage" value="$data[6]">
  </div>
  <br>
  <div>
   Weiteres:
   <br>
   <textarea rows="5" cols="40"name="weiteres" wrap="virtual">$data[7]</textarea>
  </div>
END
 }
print <<END;
  <hr>
  <font color=ffffff>
   <input type="hidden" name="alias" value="$alias">
   <input type="hidden" name="tmpid" value="$tmpid">
   <input type="hidden" name="room" value="$room">
   <input type="submit" value="Daten ändern ...">
  </font>
 </form>
</body>
</html>
END
}


sub edit { # Benutzerprofil od. Chat-Optionen abspeichern
 my $method = $_[0];
 &zeit;
 if ($method eq "edit_profile") {
  my $url = $q->param('homepage');
  $url =~ s#^http://*##;
  open(PROFILE,">data/user/profiles/$alias");
   my $profile = $q->param('nachname')."\n".$q->param('vorname')."\n".$q->param('alter')."\n".$q->param('wohnort')."\n".$q->param('email')."\n".$q->param('icq')."\n"."http://$url\n".$q->param('weiteres');
   $profile =~ s#<#&lt\;#g;
   print PROFILE $profile;
  close PROFILE;
  &edit_html("edit_profile_html","edit_profile_ok");
 } elsif ($method eq "edit_options") {
  if ($id ne $tmp) {
   &edit_html("edit_options_html","edit_pw_wrong");
   exit;
  }
  open(OPTIONS,">data/user/options/$alias");
   my $options = $id."\n".$col."\n".$secol."\n".$q->param("maxVerlauf")."\n".$q->param("verlaufOrder");
   $options =~ s#<#&lt\;#g;
   print OPTIONS $options;
  close OPTIONS;
  &edit_html("edit_options_html","edit_options_ok");
 }
}