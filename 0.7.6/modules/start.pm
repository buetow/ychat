sub start {
    if ($var eq 'create_alias_html') {
        &create_alias_html;
    }
    elsif ($var eq 'create_alias') {
        &create_alias;
    }
    elsif ($var eq 'login_html') {
        &login_html;
    }
    elsif ($var eq 'chatiquette_html') {
        &chatiquette_html;
    }
    elsif ($alias eq '') {
        &login_html;
    }
    else {
        &checklogin;
    }
}
sub login_html {
    &check_online;
    my $counter = undef;
    open COUNT, '<data/count';
    $counter = <COUNT>;
    close COUNT;
    ++$counter;
    open COUNT, '>data/count';
    flock COUNT, 2;
    print COUNT $counter;
    close COUNT;

    my(@users) = &get_online_users;

    opendir REGISTRED, 'data/user/options';
    my(@registred) = readdir REGISTRED;
    closedir REGISTRED;

    my($logtin, $users) = ($#users + 1, $#registred - 1);
    &start_html("Created & Copyright by Paul C. B\374tow", 'start');
    print qq[ <center>\n  <table width=500>\n   <tr>\n    <td>\n     <center>\n      <table>\n       <tr>\n        <td>\n         <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=5,0,0,0" WIDTH=240 HEIGHT=240>\n          <param name=movie value="$gfxpath/yIntro.swf">\n          <param name=loop value=false>\n          <param name=menu value=false>\n          <param name=quality value=high>\n          <param name=scale value=noborder>\n          <param name=wmode value=opaque>\n          <param name=bgcolor value=$bgcolor>\n          <embed src="$gfxpath/yIntro.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=$bgcolor WIDTH=240 HEIGHT=240 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>\n         </object>\n        </td>\n        <td>\n         <div>\n          yChat: Version $version\n          <br><br>\n          Letztes Update: $datum\n          <br><br>\n          Seitenaufrufe: $counter\n          <br><br>\n          Benutzer registriert: $users\n          <br><br>\n          Benutzer online: $logtin\n          <br><br>\n          Benutzerlimit: $limit\n          <br><br>\n   Page: <a href="http://www.yChat.org">www.yChat.org</a>\n <br><br><a href="http://www.webchat.de" target="_new"> <img src="http://www.webchat.de/webchat.gif" alt="[webchat]" title="[webchat]" border="0"></a><br>\n<img src="http://stats.topwebmaster.net/extern/de/snooper"></div></td>\n       </tr>\n      </table>\n     </center>\n    </div>\n    <br>\n    <br>\n];
    if ($tmp eq 'true') {
        print qq[     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Wahrscheinlich existiert Dein Alias "$alias" nicht,\n      <br>\n      Du hast Dich verschrieben oder das Passwort ist falsch.\n      <br>\n      Versuch es erneuert !\n     </div>\n];
    }
    elsif ($tmp eq 'quit') {
        print qq[     <div>\n      Du hast soeben den Chat verlassen.\n      <br>\n      Um den Chat erneuert zu betreten, einfach Daten eingeben und auf "Los Geht's" klicken :-)\n     </div>\n];
    }
    elsif ($tmp eq 'autologout') {
        print qq[     <div>\n      Du wurdest automatisch aus dem Chat ausgeloggt. Grund: Du warst 20 Minuten inaktiv.\n      <br>\n      Um den Chat erneuert zu betreten, einfach Daten eingeben und auf "Los Geht's" klicken :-)\n     </div>\n];
    }
    elsif ($tmp eq 'autokick') {
        print qq[     <div>\n      Du wurdest automatisch aus dem Chat katapultiert. Grund: Du hast den Chat geflutet!\n      <br>\n      Um den Chat erneuert zu betreten, einfach Daten eingeben und auf "Los Geht's" klicken :-)\n     </div>\n];
    }
    elsif ($tmp eq 'banned') {
        print "     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Du wurdest aus dem Chat verbannt!!!\n      <br>\n      Z.z. kannst Du Dich leider nicht in den Chat einloggen.\n      <br>\n      Sorry!\n     </div>\n";
    }
    elsif ($tmp eq 'kick') {
        print "     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Du wurdest soeben aus dem Chat katapultiert!\n      <br>\n      Schlechtes Verhalten mag die Ursache hierf\374r sein.\n      <br>\n      Sorry!\n     </div>\n";
    }
    elsif ($tmp eq 'selfkill') {
        print qq[     <div>\n      Du hast dich soeben ganz und gar aus dem System gel\366scht!\n      <br>\n      Wenn du dir einen neuen Nick angelen willst, dann einfach auf "Registrieren" klicken! :-)\n     </div>\n];
    }
    elsif ($tmp eq 'selfkill') {
        print "     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Du wurdest soeben aus dem Chat ausgeloggt!\n      <br>\n      Ursache hierf\374r mag sein, da\337 der Chat voll war und Du Dich nicht am\n      Geschehen beteiligt- und unn\366tige Chatkapazit\344ten in Anspruch\n      genommen hast.\n      <br>\n      Sorry!\n     </div>\n";
    }
    elsif ($tmp eq 'full') {
        print "<div class=b>ACHTUNG!</div><div>Der Chat ist leider voll! Bitte versuch Dich sp\344ter noch mal einzuloggen!</div>";
    }
    elsif ($var eq 'create_alias') {
        print q[<div>Du hast sich soeben in die Datenbank eingetragen. Einfach auf  "Los Geht's" klicken :-)</div>];
    }
    else {
        print qq[     <div>\n      Herzlich willkommen!\n      <br>\n      Einfach Daten eingeben und auf "Los Geht's" klicken :-)\n     </div>\n];
    }
    print qq[ <hr>\n <form name="login" action="chat" method="GET">\n <input type="hidden" name="request" value="start">\n <input type="hidden" name="tmp" value="true">\n <table>\n  <tr>\n   <td>\n    <div>\n     Dein Alias:\n     <br>\n     <input name="alias" value="$alias">\n    </div>\n   </td>\n   <td>\n    <div>\n     Dein Passwort:\n     <br>\n     <input type="password" name="pw" value="$pw">\n    </div>\n   </td>\n  </tr>\n </table>\n <br>\n <br>\n <div>\n  Solltest Du noch keinen Nick besitzen, dann auf "Registrieren" klicken!\n </div>\n <br>\n <table>\n  <tr>\n   <td>\n    <font color="ffffff">\n     <input type="submit" value="Los geht's!">\n    </font>\n </form>\n   </td>\n   <td>\n <form action="chat">\n    <input type="hidden" name="request" value="start">\n    <input type="hidden" name="var" value="create_alias_html">\n     <font color="ffffff">\n      <input type="submit" value="Registrieren">\n     </font>\n </form>\n    </td>\n   </tr>\n </table>\n <hr>\n];
    if ($logtin > 0) {
        print q[<div>Folgende Personen sind eingeloggt:<br><font size='1'>];
        foreach $_ (@users) {
            print "$_ " if -f "data/online/users/$_";
        }
        print '</font></div>';
    }
    print "    </td>\n   </tr>\n  </table>\n </center>\n</body>\n</html>\n";
    unless (-d 'data/online/rooms') {
        mkdir 'data/online/rooms', 511;
    }
}
sub create_alias_html {
    open BAN, '<data/ban';
    my(@ban) = <BAN>;
    close BAN;
    foreach $_ (@ban) {
        chomp $_;
        if ($_ eq 'ip://' . $client->peerhost) {
            $tmp = 'banned';
            &login_html;
            $client->shutdown(2);
            exit 0;
        }
    }
    my $reload = $_[0];
    &start_html('Registration');
    print " <center>\n  <table width=500>\n   <tr>\n    <td>\n";
    if ($reload eq 'alias_exists') {
        print qq[     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Der von Dir gew\344hlte Benutzername "$alias" existiert in unserer Datenbank bereits. Bitte versuch es erneuert mit\n      einem anderen Benutzernamen !\n     </div>\n     <hr>\n];
    }
    elsif ($reload eq 'pw_wrong') {
        print "     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Deine beiden Passworteingaben stimmen nicht \374berein. Bitte geb das Passwort erneuert an um sicherzugehen, da\337\n      kein falsches Passwort abgespeichert wird !\n     </div>\n     <hr>\n";
    }
    elsif ($reload eq 'alias_prohibited') {
        print qq[     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Dein gew\344hlter Benutername "$alias" enth\344lt illegale Zeichen. Bitte versuch einen anderen Benutzernamen!\n     </div>\n     <hr>\n];
    }
    elsif ($reload eq 'alias_length') {
        print qq[     <div class=b>\n      ACHTUNG!\n     </div>\n     <div>\n      Dein gew\344hlter Benutername "$alias" enth\344lt zu viele Zeichen. Es sind maximal 15 Zeichen erlaubt!!\n     </div>\n     <hr>\n];
    }
    else {
        print "     <div>\n      Hier kannst Du Dich registrieren. Bitte beachte, da\337 die Passworteingabefelder \374bereinstimmen!\n     </div>\n     <hr>\n";
    }
    print qq[     <form action="chat">\n      <input type="hidden" name="request" value="start">\n      <input type="hidden" name="var" value="create_alias">\n      <div>\n       Dein Alias:\n       <font color="ffa500">\n        (Mu\337 angegeben werden)\n       </font>\n       <br>\n       <input name="alias" VALUE="$alias">\n      </div>\n      <br>\n      <div>\n       Dein gew\374nschtes Passwort:\n       <br>\n       <input type="password" name="pw" value="$pw">\n      </div>\n      <br>\n      <div>\n       Geb hier Dein Passwort erneuert ein:\n       <br>\n       <input type="password" name="tmp">\n      </div>\n      <br>\n      <font color=ffffff>\n       <input type="submit" value="Registrieren">\n      </font>\n      <br>\n    </form>\n    <hr>\n    <div>\n     Um den Chat zu betreten m\374ssen die Daten nach der Registration auf der Startseite erneuert best\344tigt werden !\n    </div>\n   </td>\n  </tr>\n </table>\n</center>\n</body>\n</html>\n];
}
sub checklogin {
    opendir USERS, 'data/online/users';
    my(@users) = readdir USERS;
    closedir USERS;
    my $users = $#users;
    if ($users > $limit && 3 > &hierachie($alias)) {
        $tmp = 'full';
        &login_html;
        $client->shutdown(2);
        exit 0;
    }
    my $lognotexists = 'true';
    opendir USERDIR, 'data/user/options';
    my(@userdir) = readdir USERDIR;
    closedir USERDIR;
    foreach $user (@userdir) {
        if ($user eq $alias) {
            open OPTIONS, "<data/user/options/$alias";
            my(@options) = <OPTIONS>;
            close OPTIONS;
            foreach $_ (@options) {
                chomp $_;
            }
            if ($options[0] eq $pw) {
                open BAN, '<data/ban';
                my(@ban) = <BAN>;
                close BAN;
                foreach $_ (@ban) {
                    chomp $_;
                    if ($_ eq $alias or $_ eq 'ip://' . $client->peerhost) {
                        if (-f "data/online/users/$admin") {
                            &post_prv($admin, "<font color=ffffff><font color=ffa500>$_</font> kann sich nicht einloggen (Ist verbannt)!!</font>");
                        }
                        $tmp = 'banned';
                        &login_html;
		        $client->shutdown(2);
                        exit 0;
                    }
                }
                if ($options[1] eq undef) {
                    $col = &random_color;
                }
                else {
                    $col = $options[1];
                }
                if ($options[2] eq undef) {
                    $secol = &random_color;
                }
                else {
                    $secol = $options[2];
                }
                if ($options[3] eq undef) {
                    $maxVerlauf = '10';
                }
                else {
                    $maxVerlauf = $options[3];
                }
                if ($options[4] eq undef) {
                    $verlaufOrder = '1';
                }
                else {
                    $verlaufOrder = $options[4];
                }
                &rm_if_loggedin($alias);
		my(@digit) = (('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'), ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'), (0, 1, 2, 3, 4, 5, 6, 7, 8, 9), '-', '.');
                my $num1 = rand @digit;
                my $num2 = rand @digit;
                my $num3 = rand @digit;
                my $num4 = rand @digit;
                my $num5 = rand @digit;
                my $num6 = rand @digit;
                my $num7 = rand @digit;
                my $num8 = rand @digit;
                my $num9 = rand @digit;
                my $num10 = rand @digit;
                $tmpid = $digit[$num1] . $digit[$num2] . $digit[$num3] . $digit[$num4] . $digit[$num5] . $digit[$num6] . $digit[$num7] . $digit[$num8] . $digit[$num9] . $digit[$num10];
                open TMPID, ">data/online/tmpid/$alias.$tmpid";
                print TMPID $alias;
                close TMPID;
                open IDENT, ">data/online/ident/$alias";
                print IDENT $client->peerhost . "\n$tmpid\n" . $client->peerhost . ':' . $client->peerport . "\n$col\n$secol";
                close IDENT;
                open LASTLOGIN, ">data/user/lastlogin/$alias";
                print LASTLOGIN time;
                close LASTLOGIN;
                &frameset_html;
                $client->shutdown(2);
                exit 0;
            }
            else {
                $lognotexists = 'false';
            }
        }
    }
    &login_html;
}
sub create_alias {
    if ($alias =~ /\W/ or $alias eq '') {
        goto CREATE if $alias =~ /[-]/ and not $alias =~ /\W/;
        &create_alias_html('alias_prohibited');
        $client->shutdown(2);
        exit 0;
    }
    elsif ($pw ne $tmp) {
        &create_alias_html('pw_wrong');
        $client->shutdown(2);
        exit 0;
    }
    elsif (length $alias >= 15) {
        &create_alias_html('alias_length');
        $client->shutdown(2);
        exit 0;
    }
    CREATE: opendir USERDIR, 'data/user/options';
    my(@user) = readdir USERDIR;
    close USERDIR;
    foreach $user (@user) {
        if ($user eq $alias) {
            &create_alias_html('alias_exists');
            $client->shutdown(2);
            exit 0;
        }
    }
    open OPTIONS, ">data/user/options/$alias";
    print OPTIONS $pw . "\n";
    close OPTIONS;
    &login_html;
}
sub frameset_html {
    print qq[<html>\n <head>\n  <title>\n   $title\n  </title>\n </head>\n<frameset border=0 frameborder=0 framespacing=0 cols="130,*">\n <frameset border=0 frameborder=0 framespacing=0 rows="*,55">\n  <frame name="online" src="chat?request=online&room=$loginplace&alias=$alias&tmpid=$tmpid">\n  <frameset border=0 frameborder=0 framespacing=0 cols="70,*">\n   <frame name="logo" src="chat?request=htdocs&var=logo_html" scrolling="no">\n   <frame name="reload" src="chat?request=htdocs" scrolling="no">\n  </frameset>\n </frameset>\n <frameset border=0 frameborder=0 framespacing=0 rows="*,55">\n  <frame name='main' src='chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder'>\n  <frame name="input" src="chat?request=htdocs" scrolling="no">\n </frameset>\n</frameset>\n</html>\n];
}
1;
