#!/usr/bin/perl -w


use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen

#####################################
##WELCHE SUB MU� AUSGEF�HRT WERDEN?##
#####################################

if ($var eq "list_alias_html") {
 &list_alias_html;
} elsif ($var eq "alias_html") { 
 &alias_html;
} elsif ($var eq "edit_profile_html") { 
 &edit_html("edit_profile_html");
} elsif ($var eq "edit_options_html") { 
 &edit_html("edit_options_html");
} elsif ($var eq "gb_html") { 
 &gb_html;
} elsif ($var eq "post_gb") {
 &post_gb;
} elsif ($var eq "rm_gb_alias") {
 &rm_gb;
} elsif ($var eq "edit_profile") {
 &edit("edit_profile");
} elsif ($var eq "edit_options") {
 &edit("edit_options");
} else {
 &error("Parameter fehlt");
}

###########################
##HAUPTTEIL DIESER DATEI:##
###########################

sub gb_html { # Das Benutzerboard/G�stebuch.
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
  Vielen Dank f�r Deinen Beitrag. Wenn Du willst, kannst Du jetzt gleich noch einen Eintrag posten ...
 </div>
 <hr>
END
} else {
print <<END;
 <div>
  Falls Du Anregungen oder Verbesserungsvorschl�ge f�r die Community hast, dann poste doch einen kleinen Eintrag :-)
 </div>
 <hr>
END
}
print <<END;
 <form action="main2.pl">
  <input type="hidden" name="var" value="post_gb">
  <input type="hidden" name="alias" value="$alias">
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
  open (DATEI,"<data/guestbook") || &error("Konnte dat/guestbook nicht �ffnen");
   @datei = <DATEI>;
  close DATEI;
  print
   $q->div(@datei);	
 }
 print
  $q->end_html;
}


sub post_gb { # Wenn etwas in ein G�stebuch eingetragen wird
 &secure_checkip($alias);
 &zeit;
 if ($msg eq "") {
  if ($id eq "y") { # �berpr�fen, ob Benutzerboard- oder Privatg�stebuch
   &gb_html("post_empty");
  } else {
   &alias_html("post_empty");
  }
  exit;
 }
 $msg =~ s#<#&lt\;#g;
 my $tmpath = "";
 if ($id eq "y") {
  $tmpath = "data/guestbook";
 } else {
  $tmpath = "data/user/gbooks/$tmp";
 }
 if (-e "$tmpath") {
  &read_file("$tmpath");
   my @datei = <DATEI>;
  close DATEI;
  pop(@datei) if ($#datei > 99);
  my @newdatei = "<br><b>$alias schrieb am $day.$month.$year um $hours:$min:$sec ...</b><br>$msg<br>\n";
  @newdatei = ("@newdatei", "@datei");
  &write_file_new("$tmpath");
   print DATEI @newdatei;
  close DATEI;
 } else {
  &write_file_new("$tmpath");
   print DATEI "<br><b>$alias schrieb am $day.$month.$year um $hours:$min:$sec ...</b><br>$msg<br>\n";
  close DATEI;
 }
 if ($id eq "y") {
  &gb_html("post_ok");
  exit;
 } else {
  post_prv("$tmp","<font size=2 color=ffffff><a href=\"main2.pl?alias=$alias&var=alias_html&tmp=$alias\" target=\"_new\">$alias</a> hat soeben einen Pin in <a href=\"main2.pl?alias=$alias&var=alias_html&tmp=$tmp\" target=\"_new\">Dein Profil</a> geh�ngt ... </font>");
  &alias_html;
  exit;
 }
}

sub rm_gb { # G�stebucheintrag entfernen
 &secure_checkip($alias);
 $tmp = $alias;
 &read_file("data/user/gbooks/$alias");
  my @gbfile = <DATEI>;
 close DATEI;
 my @newgbfile = ();
  for($i=0; $i <= $#gbfile; $i++) {
   push(@newgbfile,$gbfile[$i]) if ($i != $id);
  }
  &write_file_new("data/user/gbooks/$alias");
   print DATEI @newgbfile;
  close DATEI;
 &alias_html("rm_ok");
}

sub list_alias_html { # Liste aller Benutzer
 &start_html("Alle Benutzer");
 &list_dir("data/user/options");
 my @liste = sort(@dir);
 my $nr = $#dir-1;
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
    $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$_" },
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
 &start_html($tmp);
print <<END;
<div>
 <a href="htdocs.pl?var=werda_html&alias=$alias">Hier</a>
 gehts zur Gesamt�bersicht !
</div>
<hr>
END
if ($message eq "rm_ok") {
print <<END;
 <div class=b>
  Meldung:
 </div>
 <div>
  Der von Dir gew�hlte Eintrag wurde erfolgreich gel�scht.
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
 &read_file("data/user/profiles/$tmp");
  @data = <DATEI>;
 close DATEI;
print <<END;
  <div>
   Alias: $tmp
   <br>
   <br>
   Nachname: $data[0]
   <br>
   <br>
   Vorname: $data[1]
   <br>
   <br>
   Alter: $data[2]
   <br>
   <br>
   Wohnort: $data[3]
   <br>
   <br>
   E-Mail:
   <a href="mailto:$data[4]">$data[4]</a>
   <br>
   <br>
   ICQ-Nummer: $data[5]
   <br>
   <br>
   Homepage:
   <a href="$data[6]" target="_new">$data[6]</a>
   <br>
   <br>
   Weiteres:
   <br>
   $data[7]
   <hr>
   Hier kannst Du $tmp eine Nachricht hinterlassen!
   <form action="main2.pl">
    <input type="hidden" name="var" value="post_gb">
    <input type="hidden" name="alias" value="$alias">
    <input type="hidden" name="tmp" value="$tmp">
    <input type="text" size="40" name="msg">
    <font color="ffffff">
     <input type="submit" value="Hinterlassen ...">
    </font>
   </form>
   <hr>
END
if (-e "data/user/gbooks/$tmp") {
  open (DATEI,"<data/user/gbooks/$tmp") || &error("Konnte data/user/gbooks/$tmp nicht �ffnen");
   @datei = <DATEI>;
  close DATEI;
  if ($alias ne $tmp) {
   print @datei;
  } else {
print <<END;
   <table>
    <form action="main2.pl">
     <input type="hidden" name="var" value="rm_gb_alias">
     <input type="hidden" name="alias" value="$alias">
END
for($i=0; $i <= $#datei; $i++) {
print <<END;
     <tr>
      <td>
       <input type="radio" name="id" value="$i">
      </td>
      <td>
       <div class=q>
        $datei[$i]
       </div>
      </td>
     </tr>
END
}
print <<END;
    </table>
    <table>
     <tr>
      <td>
       <font color=ffffff>
        <input type="submit" value="Makierten Eintrag l�schen">
       </font>
      </td>
      <td>
       <font color=ffffff>
        <input type="reset" value="Makierung r�ckg�ngig machen">
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
 &secure_checkip($alias);
 &start_html("Benutzerprofil �ndern");
 if ($method eq "edit_options_html") {
  &read_file("data/user/options/$alias");
 } elsif ($method eq "edit_profile_html") {
  &read_file("data/user/profiles/$alias");
 }
 @data = <DATEI>;
 close DATEI;
 if ($method eq "edit_options_html") {
  chomp($data[0]);
  if ($message eq "edit_options_ok") {
print <<END;
<script language="JavaScript">
 open("main.pl?var=input_html&alias=$alias&room=$room&col=$col&secol=$secol","input");
 open("htdocs.pl?var=menu_html&alias=$alias&room=$room&col=$col&secol=$secol","menu");
</script>
<div class=b>
 Hinweis:
</div>
<div>
 Du hast soeben Deine Chat-Optionen erfolgreich editiert. Hier kannst Du Dein Profil erneuert editieren. Um
 weiterzuchatten einfach auf <a href="push.pl?alias=$alias&room=$room">Weiterchatten</a> klicken.
</div>
<hr>
END
  } elsif ($message eq "edit_pw_wrong") {
print <<END;
<div class=b>
 ACHTUNG!
</div>
<div>
 Du hast soeben versucht, dein Passwort zu �ndern. Jedoch stimmte die Erste Eingabe nicht mit der Zweiten �berein.
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
<form action="main2.pl">
 <input type="hidden" name="alias" value="$alias">
 <input type="hidden" name="var" value="edit_options">
 <div>
  Dein Passwort
  <font color=ffa500>
   (Gut merken !!)
  </font>
  <br>
  <input type="password" name="id" value="$data[0]">
 </div>
 <br>
 <div>
  Geb hier das Passwort erneuert ein:
  <br>
  <input type="password" name="tmp" value="$data[0]">
 </div>
 <br>
 <div>
  "Deine Farbe (Farbe Deines Nicks, Angabe in Hexadezimal):
  <br>
  <input type="text" name="col" value="$data[1]">
 </div>
 <br>
 <div>
  Deine zweite Farbe (Farbe Deiner Nachrichten, Angabe in Hexadezimal):
  <br>
  <input type="text" name="secol" value="$data[2]">
 </div>
 <br>
END
 } elsif ($method eq "edit_profile_html") {
  if ($message eq "edit_profile_ok") {
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
  <input type="hidden" name="alias" value="$alias">
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
   <input type="submit" value="Daten �ndern ...">
  </font>
 </form>
</body>
</html>
END
}


sub edit { # Benutzerprofil od. Chat-Optionen abspeichern
 &secure_checkip($alias);
 my $method = $_[0];
 if ($method eq "edit_profile") {
  &write_file_new("data/user/profiles/$alias");
   print DATEI $q->param('nachname')."\n".$q->param('vorname')."\n".$q->param('alter')."\n".$q->param('wohnort')."\n".$q->param('email')."\n".$q->param('icq')."\n".$q->param('homepage')."\n".$q->param('weiteres');
  close DATEI;
  &log("system","\"edit_profile_ok\"");
  &edit_html("edit_profile_html","edit_profile_ok");
 } elsif ($method eq "edit_options") {
  if ($id ne $tmp) {
   &log("system","\"edit_pw_wrong\"");
   &edit_html("edit_options_html","edit_pw_wrong");
   exit;
  }
  &secure_checkip($alias);
  &write_file_new("data/user/options/$alias");
  print DATEI $id."\n".$col."\n".$secol;
  close DATEI;
  &log("system","\"edit_options_ok\"");
  &edit_html("edit_options_html","edit_options_ok");
 }
}
