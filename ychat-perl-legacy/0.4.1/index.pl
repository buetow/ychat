#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header;		# HTTP-Header erzeugen

#####################################
##WELCHE SUB MUß AUFGERUFEN WERDEN?##
#####################################
if ($var eq "create_alias_html") {
 &create_alias_html;
} elsif ($var eq "create_alias") {
 &create_alias;
} elsif ($var eq "login_html") {
 &login_html;
} elsif ($alias eq "") {
 &login_html;
} else {
 &checklogin;
}

sub login_html { # Erste Startseite
local $counter = undef;
&read_file("data/count");
 $counter = <DATEI>;
close DATEI;
$counter++;
&write_file_new("data/count");
 print DATEI $counter;
close DATEI;
&list_dir("data/online/users");
my $logtin = $#dir-1;
&start_html("Created by Paul C. Bütow 2k");
print <<END;
 <center>
  <table width=400>
   <tr>
    <td>
     <b>
      <div>
       $title - ( <a href="http://servers.yChat.de">yChat-Servers</a> )
       <img src="$gfxpath/yChat.jpg">
      </div>
     </b>
END
 if ($tmp eq "true") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Wahrscheinlich existiert Dein Alias "$alias" nicht,
      <br>
      Du hast Dich verschrieben oder das Passwort ist falsch.
      <br>
      Versuch es erneuert !
     </div>
END
 } elsif ($tmp eq "quit") {
print <<END;
     <div>
      Du hast soeben den Chat verlassen.
      <br>
      Um den Chat erneuert zu betreten, einfach Daten eingeben und auf "Los Geht's" klicken :-)
     </div>
END
 } elsif ($tmp eq "banned") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Dein Alias "$alias" wurde verbannt.
      <br>
      Z.z. kannst Du Dich leider nicht in den Chat einloggen.
      <br>
      Sorry!
     </div>
END
 } elsif ($tmp eq "kick") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Du wurdest soeben aus dem Chat katapultiert!
      <br>
      Schlechtes Verhalten mag die Ursache hierfür sein.
      <br>
      Sorry!
     </div>
END
 } elsif ($var eq "create_alias") {
print "<div>Du hast sich soeben in die Datenbank eingetragen. Einfach auf  \"Los Geht's\" klicken :-)</div>";
 } else {
print <<END;
     <div>
      Herzlich willkommen!
      <br>
      Einfach Daten eingeben und auf "Los Geht's" klicken :-)
     </div>
END
  }
print <<END;
 <hr>
 <form action="index.pl" method="post">
 <input type="hidden" name="tmp" value="true">
 <table>
  <tr>
   <td>
    <div>
     Dein Alias:
     <br>
     <input name="alias" value="$alias">
    </div>
   </td>
   <td>
    <div>
     Dein Passwort:
     <br>
     <input type="password" name="id" value="$id">
    </div>
   </td>
  </tr>
 </table>
 <br>
 <br>
 <div>
  Solltest Du noch keinen Nick besitzen, dann auf "Registrieren" klicken!
 </div>
 <br>
 <table>
  <tr>
   <td>
    <font color="ffffff">
     <input type="submit" value="Los geht's!">
    </font>
 </form>
   </td>
   <td>
 <form action="index.pl">
    <input type="hidden" name="var" value="create_alias_html">
     <font color="ffffff">
      <input type="submit" value="Registrieren">
     </font>
 </form>
    </td>
   </tr>
 </table>
 <hr>
 <div>
  yChat befindet sich momentan noch in der Entwicklungsphase !
  <br>
  Fragen gehen an Paul Bütow (
   <a href="mailto:paul\@buetow.oxg.de">
    paul\@buetow.oxg.de
   </a>
   ) :-)
   <br>
   <br>
   Diese Seite wurde schon $counter mal aufgerufen. Es sind z.Z. $logtin Benutzer online.
    </td>
   </tr>
  </table>
 </center>
</body>
</html>
END
 if ($logtin == 0) {
  &list_dir("data/msgs");
  &list_dir("data/online/rstat",2);
  foreach(@dir) {
   unlink("data/msgs/$_");
   unlink("data/online/ident/$_");
  }
  foreach(@dir2) {
   unlink("data/online/rstat/$_");
  }
 }
 # Falls d. Skript das Raumverz. gelöscht hat, neu anlegen
 if (-d "data/online/rooms") {
 } else {
  mkdir "data/online/rooms", 0777;
 }
}


sub create_alias_html { # HTML-Form für Erstellung eines neuen Benutzers.
my $reload = $_[0];
&start_html("Registration");
print <<END;
 <center>
  <table width=400>
   <tr>
    <td>
END
 if ($reload eq "alias_exists") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Der von Dir gewählte Benutzername "$alias" existiert in unserer Datenbank bereits. Bitte versuch es erneuert mit
      einem anderen Benutzernamen !
     </div>
     <hr>
END
} elsif ($reload eq "pw_wrong") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Deine beiden Passworteingaben stimmen nicht überein. Bitte geb das Passwort erneuert an um sicherzugehen, daß
      kein falsches Passwort abgespeichert wird !
     </div>
     <hr>
END
} elsif ($reload eq "alias_prohibited") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Dein gewählter Benutername "$alias" enthält illegale Zeichen. Bitte versuch einen anderen Benutzernamen!
     </div>
     <hr>
END
} else {
print <<END;
     <div>
      Hier kannst Du Dich registrieren. Bitte beachte, daß die Passworteingabefelder übereinstimmen!
     </div>
     <hr>
END
}
print <<END;
     <form action="index.pl">
      <input type="hidden" name="var" value="create_alias">
      <div>
       Dein Alias:
       <font color="ffa500">
        (Muß angegeben werden)
       </font>
       <br>
       <input name="alias" VALUE="$alias">
      </div>
      <br>
      <div>
       Dein gewünschtes Passwort:
       <br>
       <input type="password" name="id" value="$id">
      </div>
      <br>
      <div>
       Geb hier Dein Passwort erneuert ein:
       <br>
       <input type="password" name="tmp">
      </div>
      <br>
      <font color=ffffff>
       <input type="submit" value="Registrieren">
      </font>
      <br>
    </form>
    <hr>
    <div>
     Um den Chat zu betreten müssen die Daten nach der Registration auf der Startseite erneuert bestätigt werden !
    </div>
   </td>
  </tr>
 </table>
</center>
</body>
</html>
END
}

sub checklogin { # Überprüfen, ob die Login-Daten korrekt sind.
 my $lognotexists = "true";
 &list_dir("data/user/options");
 for (my $i=0; $i<=$#dir; $i++) {
  if ($dir[$i] eq $alias) {
   &read_file("data/user/options/$alias");
    @datei = <DATEI>;
   close DATEI;
   chomp($datei[0]);
   if ($datei[0] eq $id) {
    &read_file("data/ban"); # Überprüfen, ob  Alias verbannt ist
     @banned = <DATEI>;
    close DATEI;
    foreach(@banned) {
     if ($_ eq "$alias\n") {
      $tmp = "banned";
      &login_html();
      exit;
     }
    }
    $col = $datei[1];
    $secol = $datei[2];
    $tmpid = time;
    &write_file_new("data/online/ident/$alias");
     print DATEI $ENV{'REMOTE_ADDR'}."\n".$tmpid;
    close DATEI;
    &read_file("data/user/lastlogin/$alias");
     my @lastlogin = <DATEI>;
    close DATEI;
    &write_file_new("data/user/lastlogin/$alias");
     print DATEI time;
    close DATEI;
    &frameset_html;
    exit;
   } else {
    $lognotexists = "false";
   }
  }
 }
 if ($lognotexists eq "true") {
 }
 &login_html;	# Falls Login-Daten inkorrekt sind, zurück zur Startseite.
}

sub create_alias { # Erstellung eines neuen Benutzers.
 if ($alias =~ /[\W]/ || $alias eq "") {
  &create_alias_html("alias_prohibited");;
  exit;
 } elsif ($id ne $tmp) {
  &create_alias_html("pw_wrong");
  exit;
 }
 &list_dir("data/user/options");
 for (my $i=0; $i<=$#dir; $i++) {
  if ($dir[$i] eq $alias) {
   &create_alias_html("alias_exists");
   exit;
  }
 }
 &write_file_new("data/user/options/$alias");
  print DATEI $id."\n";
 close DATEI;
 &zeit;
 &list_dir("data/user/options");
 my $number = $#dir-1;
 &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias [$number] hat sich soeben als neuer Benutzer registriert :-) ...</font></i>");
 &login_html;
}

sub frameset_html {
print <<END;
<html>
 <head>
  <title>
   $title - Created & Copyright by Paul C. Bütow
  </title>
 </head>

<frameset border=0 frameborder=0 framespacing=0 cols="140,*">
<frameset border=0 frameborder=0 framespacing=0 rows="210,*,55">
 <frame name="menu" src="htdocs.pl?alias=$alias&var=menu_html&room=$room&col=$col&secol=$secol&tmpid=$tmpid" scrolling="no">
 <frame name="online" src="online.pl?room=$room&alias=$alias&tmpid=$tmpid">
 <frame name="reload" src="htdocs.pl" scrolling="no">
</frameset>
<frameset border=0 frameborder=0 framespacing=0 rows="*,55">
  <frame name="main" src="push.pl?room=$room&alias=$alias&tmpid=$tmpid">
  <frame name="input" src="main.pl?alias=$alias&var=input_html&room=$room&col=$col&secol=$secol&tmp=login&tmpid=$tmpid" scrolling="no">
</frameset>
</frameset>
</html>
END
}
