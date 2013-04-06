#!/usr/bin/perl

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print $q->header;

$var = $q->param("var");
$alias = $q->param("alias");
$pw = $q->param("pw");
$tmp = $q->param("tmp");

if ($var eq "create_alias_html") {
 &create_alias_html;
} elsif ($var eq "create_alias") {
 &create_alias;
} elsif ($var eq "login_html") {
 &login_html;
} elsif ($var eq "chatiquette_html") {
 &chatiquette_html;
} elsif ($alias eq "") {
 &login_html;
} else {
 &checklogin;
}

sub login_html { # Erste Startseite
&check_online;
local $counter = undef;
open(COUNT,"<data/count");
 $counter = <COUNT>;
close COUNT;
$counter++;
open(COUNT,">data/count");
 flock(COUNT, 2);
 print COUNT $counter;
close COUNT;
opendir(USERS,"data/online/users");
 my @users = readdir(USERS);
closedir(USERS);
opendir(REGISTRED,"data/user/options");
 local @registred = readdir(REGISTRED);
closedir(REGISTRED);
local ($logtin,$users) = ($#users-1,$#registred-1);
&start_html("Created & Copyright by Paul C. Bütow","start");
print <<END;
 <center>
  <table width=500>
   <tr>
    <td>
     <center>
      <table>
       <tr>
        <td>
         <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=5,0,0,0" WIDTH=240 HEIGHT=240>
          <param name=movie value="$gfxpath/yIntro.swf">
          <param name=loop value=false>
          <param name=menu value=false>
          <param name=quality value=high>
          <param name=scale value=noborder>
          <param name=wmode value=opaque>
          <param name=bgcolor value=#005146>
          <embed src="$gfxpath/yIntro.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=#005146 WIDTH=240 HEIGHT=240 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>
         </object>
        </td>
        <td>
         <div>
          yChat: Version $version
          <br><br>
          Letztes Update: $datum
          <br><br>
          Seitenaufrufe: $counter
          <br><br>
          Benutzer registriert: $users
          <br><br>
          Benutzer online: $logtin
          <br><br>
          Webmaster: <a href="mailto:mail\@paulbuetow.de">Paul C. Bütow</a>
          <br><br>
          yChat-HP: <a href="http://www.home.yChat.de">home.yChat.de</a>
         </div>
        </td>
       </tr>
      </table>
     </center>
    </div>
    <br>
    <br>
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
 } elsif ($tmp eq "autologout") {
print <<END;
     <div>
      Du wurdest automatisch aus dem Chat ausgeloggt. Grund: Du warst 20 Minuten inaktiv.
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
      Du wurdest aus dem Chat verbannt!!!
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
 <form name="login" action="index.pl" method="post">
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
     <input type="password" name="pw" value="$pw">
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
END
if ($logtin > 0) {
 print "<div>Folgende Personen sind eingeloggt:<br><font size='1'>";
 foreach(@users) {
  print "$_ " if (-f "data/online/users/$_");
 }
 print "</font></div>";
}
print<<END;
    </td>
   </tr>
  </table>
 </center>
</body>
</html>
END
 if (-d "data/online/rooms") {
 } else {
  mkdir "data/online/rooms", 0777;
 }
}


sub chatiquette_html {
&start_html("Registration");
print <<END;
 <center>
  <table width=500>
   <tr>
    <td>
END
open CHATIQ, "chatiq.sno";
 foreach(<CHATIQ>) {
  last if (/^\n+$/);
  print $_;
 }
WEITER:
close CHATIQ;
print<<END;
    </td>
   </tr>
  </table>
 </center>
</body>
</html>
END
}

sub create_alias_html { # HTML-Form für Erstellung eines neuen Benutzers.
my $reload = $_[0];
&start_html("Registration");
print <<END;
 <center>
  <table width=500>
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
} elsif ($reload eq "alias_length") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Dein gewählter Benutername "$alias" enthält zu viele Zeichen. Es sind maximal 15 Zeichen erlaubt!!
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
       <input type="password" name="pw" value="$pw">
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
 opendir(USERDIR,"data/user/options");
  my @userdir = readdir(USERDIR);
 closedir(USERDIR);
 foreach $user (@userdir) {
  if ($user eq $alias) {
   open(OPTIONS,"<data/user/options/$alias");
    my @options = <OPTIONS>;
   close OPTIONS;
   foreach(@options) {
    chomp $_;
   }
   if ($options[0] eq $pw) {
    open(BAN,"<data/ban");
     my @ban = <BAN>;
    close BAN;
    foreach(@ban) {
     chomp;
     if ($_ eq "$alias" || $_ eq "ip://".$ENV{'REMOTE_ADDR'}) {
      $tmp = "banned";
      &login_html;
      exit 0;
     }
    }
    if ($options[1] eq undef) { $col = &random_color;}
    else { $col = $options[1]; }
    if ($options[2] eq undef) { $secol = &random_color;}
    else { $secol = $options[2];}
    if ($options[3] eq undef) { $maxVerlauf = "10";}
    else { $maxVerlauf = $options[3];}
    if ($options[4] eq undef) { $verlaufOrder = "1";}
    else               {$verlaufOrder = $options[4];}
    my @digit = (A..Z, a..z, 0..9, "-", ".");
    my $num1 = rand(@digit);
    my $num2 = rand(@digit);
    my $num3 = rand(@digit);
    my $num4 = rand(@digit);
    my $num5 = rand(@digit);
    my $num6 = rand(@digit);
    my $num7 = rand(@digit);
    my $num8 = rand(@digit);
    my $num9 = rand(@digit);
    my $num10 = rand(@digit);
    $tmpid = $digit[$num1].$digit[$num2].$digit[$num3].$digit[$num4].$digit[$num5].$digit[$num6].$digit[$num7].$digit[$num8].$digit[$num9].$digit[$num10];
    open TMPID, ">data/online/tmpid/$alias.$tmpid";
     print TMPID $alias;
    close TMPID;
    open IDENT, ">data/online/ident/$alias" ;
     print IDENT "$ENV{'REMOTE_ADDR'}\n$tmpid\n$ENV{'HTTP_USER_AGENT'}";
    close IDENT;
    open LASTLOGIN, ">data/user/lastlogin/$alias";
     print LASTLOGIN time;
    close LASTLOGIN;
    &frameset_html;
    exit 0;
   } else {
    $lognotexists = "false";
   }
  }
 }
 &login_html;        # Falls Login-Daten inkorrekt sind, zurück zur Startseite.
}

sub create_alias { # Erstellung eines neuen Benutzers.
 if ($alias =~ /\W/ || $alias eq "") {
 goto CREATE  if ($alias =~ /[-]/ && $alias !~ /\W/);
  &create_alias_html("alias_prohibited");;
  exit 0;
 } elsif ($pw ne $tmp) {
  &create_alias_html("pw_wrong");
  exit 0;
 } elsif (length($alias) >= 15) {
  &create_alias_html("alias_length");
  exit 0;
 }
CREATE:
 opendir(USERDIR,"data/user/options");
  my @user = readdir(USERDIR);
 close USERDIR;
 foreach $user (@user) {
  if ($user eq $alias) {
   &create_alias_html("alias_exists");
   exit;
  }
 }
 open(OPTIONS,">data/user/options/$alias");
  print OPTIONS $pw."\n";
 close OPTIONS;
 local $number = $#user;
 &zeit;
 &post("Cyberbar","<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffffff>$alias [$number] hat sich soeben als neuer Benutzer registriert :-) ...</font></i>");
 &login_html;
}

sub frameset_html {
print <<END;
<html>
 <head>
  <title>
   $title - Powered by CXG - Chatters of the next generation
  </title>
 </head>
<frameset border=0 frameborder=0 framespacing=0 cols="130,*">
 <frameset border=0 frameborder=0 framespacing=0 rows="300,*,55">
  <frame name="online" src="online.pl?room=Cyberbar&alias=$alias&tmpid=$tmpid">
  <frame src="htdocs.pl">
  <frameset border=0 frameborder=0 framespacing=0 cols="70,*">
   <frame name="logo" src="htdocs.pl?var=logo_html" scrolling="no">
   <frame name="reload" src="htdocs.pl" scrolling="no">
  </frameset>
 </frameset>
 <frameset border=0 frameborder=0 framespacing=0 rows="60,*,55">
  <frame name="menu" src="htdocs.pl?alias=$alias&var=menu_html&room=Cyberbar&col=$col&secol=$secol&tmpid=$tmpid" scrolling="no">
  <frame name='main' src='push.pl?room=Cyberbar&alias=$alias&tmpid=$tmpid'>
  <frame name="input" src="input.pl?alias=$alias&room=Cyberbar&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder" scrolling="no">
 </frameset>
</frameset>
</html>
END
}