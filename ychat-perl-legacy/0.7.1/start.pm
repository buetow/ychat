
sub start {

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
my ($logtin,$users) = ($#users-1,$#registred-1);
&start_html("Created & Copyright by Paul C. B�tow","start");
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
          <param name=bgcolor value=$bgcolor>
          <embed src="$gfxpath/yIntro.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=$bgcolor WIDTH=240 HEIGHT=240 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>
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
          Benutzerlimit: $limit
          <br><br>
          Server: $localhost:$localport
          <br><br>
          Page: <a href="http://www.yChat.org">www.yChat.org</a>
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
 } elsif ($tmp eq "autokick") {
print <<END;
     <div>
      Du wurdest automatisch aus dem Chat katapultiert. Grund: Du hast den Chat geflutet!
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
      Schlechtes Verhalten mag die Ursache hierf�r sein.
      <br>
      Sorry!
     </div>
END
 } elsif ($tmp eq "selfkill") {
print <<END;
     <div>
      Du hast dich soeben ganz und gar aus dem System gel�scht!
      <br>
      Wenn du dir einen neuen Nick angelen willst, dann einfach auf "Registrieren" klicken! :-)
     </div>
END
 } elsif ($tmp eq "selfkill") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Du wurdest soeben aus dem Chat ausgeloggt!
      <br>
      Ursache hierf�r mag sein, da� der Chat voll war und Du Dich nicht am
      Geschehen beteiligt- und unn�tige Chatkapazit�ten in Anspruch
      genommen hast.
      <br>
      Sorry!
     </div>
END
 } elsif ($tmp eq "full") {
 print "<div class=b>ACHTUNG!</div><div>Der Chat ist leider voll! Bitte versuch Dich sp�ter noch mal einzuloggen!</div>";
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
 <form name="login" action="chat" method="GET">
 <input type="hidden" name="request" value="start">
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
 <form action="chat">
    <input type="hidden" name="request" value="start">
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

sub create_alias_html { # HTML-Form f�r Erstellung eines neuen Benutzers.
 open(BAN,"<data/ban");
  my @ban = <BAN>;
 close BAN;
 foreach(@ban) {
  chomp;
  if ($_ eq "ip://".$client->peerhost()) {
   $tmp = "banned";
   &login_html;
   $client->shutdown(2);
   exit 0;
  }
 }
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
      Der von Dir gew�hlte Benutzername "$alias" existiert in unserer Datenbank bereits. Bitte versuch es erneuert mit
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
      Deine beiden Passworteingaben stimmen nicht �berein. Bitte geb das Passwort erneuert an um sicherzugehen, da�
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
      Dein gew�hlter Benutername "$alias" enth�lt illegale Zeichen. Bitte versuch einen anderen Benutzernamen!
     </div>
     <hr>
END
} elsif ($reload eq "alias_length") {
print <<END;
     <div class=b>
      ACHTUNG!
     </div>
     <div>
      Dein gew�hlter Benutername "$alias" enth�lt zu viele Zeichen. Es sind maximal 15 Zeichen erlaubt!!
     </div>
     <hr>
END
} else {
print <<END;
     <div>
      Hier kannst Du Dich registrieren. Bitte beachte, da� die Passworteingabefelder �bereinstimmen!
     </div>
     <hr>
END
}
print <<END;
     <form action="chat">
      <input type="hidden" name="request" value="start">
      <input type="hidden" name="var" value="create_alias">
      <div>
       Dein Alias:
       <font color="ffa500">
        (Mu� angegeben werden)
       </font>
       <br>
       <input name="alias" VALUE="$alias">
      </div>
      <br>
      <div>
       Dein gew�nschtes Passwort:
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
     Um den Chat zu betreten m�ssen die Daten nach der Registration auf der Startseite erneuert best�tigt werden !
    </div>
   </td>
  </tr>
 </table>
</center>
</body>
</html>
END
}

sub checklogin { # �berpr�fen, ob die Login-Daten korrekt sind.
opendir(USERS,"data/online/users");
 my @users = readdir(USERS);
closedir(USERS);
 my $users = $#users;
 my $hierachie = &hierachie($alias);
 if ($users > $limit && $hierachie ne "wa" && $hierachie ne "ow") {
  $tmp = "full";
  &login_html;
  exit 0;
 }
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
     if ($_ eq $alias || $_ eq "ip://".$client->peerhost()) {
      if (-f "data/online/users/$admin") {
       &post_prv($admin,"<font color=ffffff><font color=ffa500>$_</font> kann sich nicht einloggen (Ist verbannt)!!</font>");
      }
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
     print IDENT $client->peerhost()."\n$tmpid\n".$client->peerhost().":".$client->peerport();
    close IDENT;
    open LASTLOGIN, ">data/user/lastlogin/$alias";
     print LASTLOGIN time;
    close LASTLOGIN;
    &frameset_html;
    $client->shutdown(2);
    exit 0;
   } else {
    $lognotexists = "false";
   }
  }
 }
 &login_html;        # Falls Login-Daten inkorrekt sind, zur�ck zur Startseite.
}

sub create_alias { # Erstellung eines neuen Benutzers.
 if ($alias =~ /\W/ || $alias eq "") {
 goto CREATE  if ($alias =~ /[-]/ && $alias !~ /\W/);
  &create_alias_html("alias_prohibited");;
   $client->shutdown(2);
   exit 0;
 } elsif ($pw ne $tmp) {
  &create_alias_html("pw_wrong");
  $client->shutdown(2);
  exit 0;
 } elsif (length($alias) >= 15) {
  &create_alias_html("alias_length");
  $client->shutdown(2);
  exit 0;
 }
CREATE:
 opendir(USERDIR,"data/user/options");
  my @user = readdir(USERDIR);
 close USERDIR;
 foreach $user (@user) {
  if ($user eq $alias) {
   &create_alias_html("alias_exists");
   $client->shutdown(2);
   exit 0;
  }
 }
 open(OPTIONS,">data/user/options/$alias");
  print OPTIONS $pw."\n";
 close OPTIONS;

 &login_html;
}

sub frameset_html {
print <<END;
<html>
 <head>
  <title>
   $title
  </title>
 </head>
<frameset border=0 frameborder=0 framespacing=0 cols="130,*">
 <frameset border=0 frameborder=0 framespacing=0 rows="*,55">
  <frame name="online" src="chat?request=online&room=$loginplace&alias=$alias&tmpid=$tmpid">
  <frameset border=0 frameborder=0 framespacing=0 cols="70,*">
   <frame name="logo" src="chat?request=htdocs&var=logo_html" scrolling="no">
   <frame name="reload" src="chat?request=htdocs" scrolling="no">
  </frameset>
 </frameset>
 <frameset border=0 frameborder=0 framespacing=0 rows="*,55">
  <frame name='main' src='chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder'>
  <frame name="input" src="chat?request=htdocs" scrolling="no">
 </frameset>
</frameset>
</html>
END
}


1;
