sub divcom {
    if ($var eq 'list_alias_html') {
        &list_alias_html;
    }
    elsif ($var eq 'alias_html') {
        &alias_html;
    }
    elsif ($var eq 'edit_profile_html') {
        &secure_checkid($alias, $tmpid);
        &edit_html('edit_profile_html');
    }
    elsif ($var eq 'edit_options_html') {
        &secure_checkid($alias, $tmpid);
        &edit_html('edit_options_html');
    }
    elsif ($var eq 'gb_html') {
        &gb_html;
    }
    elsif ($var eq 'post_gb') {
        &secure_checkid($alias, $tmpid);
        &post_gb;
    }
    elsif ($var eq 'edit_profile') {
        &secure_checkid($alias, $tmpid);
        &edit('edit_profile');
    }
    elsif ($var eq 'edit_options') {
        &secure_checkid($alias, $tmpid);
        &edit('edit_options');
    }
    elsif ($var eq 'rm_gb_alias') {
        &secure_checkid($alias, $tmpid);
        &rm_gb;
    }
    else {
        &error('Parameter fehlt');
    }
}
sub gb_html {
    my ($message,$method) = @_;
    unless ($method eq 'loggedin') {
    	&start_html('Benutzerboard');
    	&menu();
    }
    if ($message eq 'post_empty') {
        print " <div class=b>\n  ACHTUNG!\n </div>\n <div>\n  Du wolltest soeben einen leeren Eintrag posten. Bitte schreib\n  <i>\n   zuerst\n  </i>\n  einen Text und poste\n  <u>dann</u>\n  !\n </div>\n <hr>\n";
    }
    elsif ($message eq 'post_ok') {
        print " <div class=b>\n  Meldung:\n </div>\n <div>\n  Vielen Dank f\374r Deinen Beitrag. Wenn Du willst, kannst Du jetzt gleich noch einen Eintrag posten ...\n </div>\n <hr>\n";
    }
    else {
        print " <div>\n  Falls Du Anregungen oder Verbesserungsvorschl\344ge f\374r die Community hast, dann poste doch einen kleinen Eintrag :-)\n </div>\n <hr>\n";
    }
    print qq[ <form action="chat">\n  <input type="hidden" name="col" value="$col">\n<input type="hidden" name="secol" value="$secol">\n<input type="hidden" name="maxVerlauf" value="$maxVerlauf">\n<input type="hidden" name="verlaufOrder" value="$verlaufOrder">\n<input type="hidden" name="request" value="divcom">\n <input type="hidden" name="method" value="$method">\n  <input type="hidden" name="var" value="post_gb">\n  <input type="hidden" name="alias" value="$alias">\n  <input type="hidden" name="tmpid" value="$tmpid">\n  <input type="hidden" name="tmp" value="guestbook">\n  <input type="hidden" name="id" value="y">\n  <input size="40" name="msg">\n  <font color="ffffff">\n   <input type="submit" value="Hinterlassen ...">\n  </font>\n </form>\n <hr>\n];
    if (-e 'data/guestbook') {
        &error("Konnte dat/guestbook nicht \366ffnen") unless open DATEI, '<data/guestbook';
        @datei = <DATEI>;
        close DATEI;
        print $q->div(@datei);
    }
    print $q->end_html;
}
sub post_gb {
    my($secs, $mins, $hours, $days, $months, $years) = &timearray;
    if ($msg eq '') {
        if ($id eq 'y') {
	    if ($method eq 'loggedin') {
                &loggedin('post_empty');
	    } else {
            	&gb_html('post_empty');
            }
        }
        else {
            &alias_html('post_empty');
        }
        $client->shutdown(2);
        exit 0;
    }
    $msg =~ s/</&lt;/g;
    $msg =~ s/\n/<br>/g;
    my $tmpath = '';
    if ($id eq 'y') {
        $tmpath = 'data/guestbook';
    }
    else {
        $tmpath = "data/user/gbooks/$tmp";
    }
    open GB, "$tmpath";
    my(@gb) = <GB>;
    close GB;
    pop @gb if $#gb > 99;
    my(@newdatei) = "<br><b>$alias schrieb am $days.$months.$years um $hours:$mins:$secs ...</b><br>$msg<br>\n";
    push @newdatei, @gb;
    open GB, ">$tmpath";
    print GB @newdatei;
    close GB;
    if ($id eq 'y') {
        if ($method eq 'loggedin') {
       		&loggedin('post_ok');
        } else {
       		&gb_html('post_ok');
        }
        $client->shutdown(2);
        exit 0;
    }
    &post_prv ($tmp, "<font color=ffffff>$alias hat soeben einen Pin in Dein Profil geh\344ngt ... </font>");
    &gb_html('post_ok');###
    $client->shutdown(2);
    exit 0;
}
sub rm_gb {
    open GB, "<data/user/gbooks/$alias";
    my(@gbfile) = <GB>;
    close GB;
    my(@newgbfile) = ();
    for ($i = 0; $i <= $#gbfile; ++$i) {
        push @newgbfile, $gbfile[$i] if $i != $id;
    }
    open GB, ">data/user/gbooks/$alias";
    print GB @newgbfile;
    close GB;
    &alias_html('rm_ok');
}
sub list_alias_html {
    &start_html('Alle Benutzer');
    opendir REGISTERED, 'data/user/options';
    my(@users) = readdir REGISTERED;
    closedir REGISTERED;
    my(@liste) = sort(@users);
    my $nr = $#users - 1;
    &menu();
    print " <div>\n  Es sind z.Z. genau $nr Benutzernamen in der Datenbank vorhanden! (Sortiert nach #, A-Z, a-z)\n </div>\n <hr>\n <div>\n";
    foreach $_ (@liste) {
        if (-f "data/user/options/$_") {
            print $q->a({-'href', "chat?request=divcom&var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid"}, "$_"), ' ';
        }
     }
     print " </div>\n</body>\n</html>\n";
}
sub alias_html {
    my $message = $_[0];
    open COUNTS, "<data/user/counts/$tmp";
    local $counter = <COUNTS>;
    close COUNTS;
    ++$counter;
    open COUNTS, ">data/user/counts/$tmp";
    print COUNTS $counter;
    close COUNTS;
    &start_html($tmp);
    &menu();
    print "";
    if ($message eq 'rm_ok') {
        print " <div class=b>\n  Meldung:\n </div>\n <div>\n  Der von Dir gew\344hlte Eintrag wurde erfolgreich gel\366scht.\n </div>\n <hr>\n";
    }
    elsif ($message eq 'post_empty') {
        print " <div class=b>\n  ACHTUNG!\n </div>\n <div>\n  Du wolltest soeben einen leeren Eintrag posten. Bitte schreib\n  <i>\n   zuerst\n  </u>\n  einen Text und poste\n  <u>dann</u>\n  !\n  <hr>\n";
    }
    open PROFILE, "<data/user/profiles/$tmp";
    @data = <PROFILE>;
    close PROFILE;
    print qq[  <div>\n   Dieses Profil wurde insgesamt schon $counter mal aufgerufen !\n   <br><br>Alias: $tmp\n   <br><br>Nachname: $data[0]\n   <br><br>Vorname: $data[1]\n   <br><br>Alter: $data[2]\n   <br><br>Wohnort: $data[3]\n   <br><br>E-Mail:\n   <a href="mailto:$data[4]">$data[4]</a>\n   <br><br>ICQ-Nummer: $data[5]\n   <br><br>Homepage: <a href="$data[6]" target="_new">$data[6]</a>\n   <br><br>Weiteres:<br>$data[7]\n   <hr>\n   Hier kannst Du $tmp eine Nachricht hinterlassen!\n   <form action="chat">\n    <input type="hidden" name="request" value="divcom">\n    <input type="hidden" name="var" value="post_gb">\n    <input type="hidden" name="alias" value="$alias">\n    <input type="hidden" name="tmpid" value="$tmpid">\n    <input type="hidden" name="tmp" value="$tmp">\n    <input type="text" size="40" name="msg">\n    <font color="ffffff">\n     <input type="submit" value="Hinterlassen ...">\n    </font>\n   </form>\n   <hr>\n];
    if (-e "data/user/gbooks/$tmp") {
        &error("Konnte data/user/gbooks/$tmp nicht \366ffnen") unless open DATEI, "<data/user/gbooks/$tmp";
        @datei = <DATEI>;
        close DATEI;
        my $nr = 1;
        if ($alias ne $tmp) {
            foreach $_ (@datei) {
                print "<br>Eintrag # $nr: $_";
                ++$nr;
            }
        }
        else {
            print qq[   <table>\n    <form action="chat">\n     <input type="hidden" name="request" value="divcom">\n     <input type="hidden" name="var" value="rm_gb_alias">\n     <input type="hidden" name="alias" value="$alias">\n     <input type="hidden" name="tmpid" value="$tmpid">\n     <input type="hidden" name="tmp" value="$tmp">\n];
            for ($i = 0; $i <= $#datei; ++$i) {
                print qq[     <tr>\n      <td>\n       <input type="radio" name="id" value="$i">\n      </td>\n      <td>\n       <div class=q>\n       <br>\n       Eintrag # $nr:\n        $datei[$i]\n       </div>\n      </td>\n     </tr>\n];
                ++$nr;
            }
            print qq[    </table>\n    <table>\n     <tr>\n      <td>\n       <font color=ffffff>\n        <input type="submit" value="Markierten Eintrag l\366schen">\n       </font>\n      </td>\n      <td>\n       <font color=ffffff>\n        <input type="reset" value="Markierung r\374ckg\344ngig machen">\n       </font>\n      </td>\n     </tr>\n    </table>\n   </form>\n];
        }
    }
    print $q->end_html;
}
sub edit_html {
    my($method, $message) = @_;
    &start_html("Benutzerprofil \344ndern");
    if ($method eq 'edit_options_html') {
        open DATEI, "<data/user/options/$alias";
    }
    elsif ($method eq 'edit_profile_html') {
        open DATEI, "<data/user/profiles/$alias";
    }
    @data = <DATEI>;
    close DATEI;
    foreach $_ (@data) {
        chomp $_;
    }
    &menu();
    if ($method eq 'edit_options_html') {
        if ($message eq 'edit_options_ok') {
            print "<div class=b>\n Hinweis:\n</div>\n<div>\n Du hast soeben Deine Chat-Optionen erfolgreich editiert. Hier kannst Du Dein Profil erneuert editieren. Um\n weiterzuchatten einfach Fenster schliessen!\n</div>\n<hr>\n";
        }
        elsif ($message eq 'edit_pw_wrong') {
            print "<div class=b>\n ACHTUNG!\n</div>\n<div>\n Du hast soeben versucht, dein Passwort zu \344ndern. Jedoch stimmte die Erste Eingabe nicht mit der Zweiten \374berein.\n Bitte versuch es erneuert ...\n</div>\n<hr>\n";
        }
        else {
            print $q->div('Hier kannst Du Deine Chat-Optionen editieren!'), $q->hr;
        }
        print qq[<script lang="javascript" type="text/javascript">\n<!--\n\nfunction radioWert(rObj) {\n  for (var i=0; i<rObj.length; i++) if (rObj[i].checked) return rObj[i].value;\n  return false;\n}\n\nfunction insertcode( code ) {\n  document.optionen.elements[radioWert(document.optionen.eingabe)].value = code;\n}\n\n//-->\n</script>\n\n<form name="optionen" action="chat">\n <input type="hidden" name="var" value="edit_options">\n <table>\n  <tr>\n   <td>\n    <div>\n     Dein Passwort\n     <font color=ffa500>\n      (Gut merken !!)\n     </font>\n     <br>\n     <input type="password" name="id" value="$data[0]">\n    </div>\n   </td>\n   <td width=20>\n   </td>\n   <td>\n    <div>\n     Geb hier das Passwort erneuert ein:\n     <br>\n     <input type="password" name="tmp" value="$data[0]">\n    </div>\n   </td>\n  </tr>\n </table>\n <hr>\n <div>Einstellungen f\374r den Nachrichten-Verlauf</div>\n <table>\n  <tr>\n   <td>\n    <br>\n    <div>Anzahl der Nachrichten im Verlauf (Standart = 10)</div>\n    <font color=ffffff>\n     <select name="maxVerlauf" size="1">\n     <option value="$data[3]">$data[3]\n];
        for ($i = 1; $i <= 50; ++$i) {
            print qq[<option value="$i">$i];
        }
        print qq[     </select>\n    </font>\n   </td>\n  </tr>\n  <tr>\n   <td>\n      <br>\n<div>Wie sollen die Nachrichten im Verlauf angeordnet sein?</div>\n    <font color=ffffff>\n     <select name="verlaufOrder" size="1">\n      <option value="$data[4]">Bisherige Einstellung ($data[4])\n      <option value="0">Neue Verlaufnachrichten oben (0)\n      <option value="1">Neue Verlaufnachrichten unten (1)\n     </select>\n    </font>\n   </td>\n  </tr>\n </table>\n<br>\n <div>PS: Einstellungen des Nachrichten-Verlaufs werden erst nach einem Neueinloggen aktualisiert.</div>\n <hr>\n <table>\n  <tr>\n   <td>\n    <div>\n     <input type="radio" name="eingabe" value="col" checked>\n      Auswahl von Farbe 1\n      <br>\n      (Farbe Deines Nicks):\n      <br>\n      <input name="col" size="6" maxlength="6" value="$data[1]">\n     </div>\n    </td>\n   <td width=20>\n   </td>\n    <td>\n    <div>\n     <input type="radio" name="eingabe" value="secol">\n      Auswahl von Farbe 2\n      <br>\n      (Farbe Deiner Nachrichten):\n      <br>\n      <input name="secol" size="6" maxlength="6" value="$data[2]">\n     </div>\n    </td>\n   </tr>\n  </table>\n<br>\n <div>\n  Beliebige Auswahl markieren und Farbe w\344hlen. Um Deine Optionen abzuspeichern,\n  auf "Daten \344ndern" klicken!\n </div>\n <br>\n];
        open COLORS, '<templates/colors.sno';
        @colors = <COLORS>;
        close COLORS;
        print @colors;
    }
    elsif ($method eq 'edit_profile_html') {
        if ($message eq 'edit_profile_ok') {
            print " <div class=b>\n  Hinweis:\n </div>\n <div>\n Du hast soeben Dein Profil erfolgreich editiert. Hier kannst Du Dein Profil erneuert editieren:\n <hr>\n";
        }
        else {
            print $q->div('Hier kannst Du Dein Profil editieren!'), $q->hr;
        }
        print qq[ <form action="chat">\n  <input type="hidden" name="var" value="edit_profile">\n  <div>\n   Dein Nachname:\n   <br>\n   <input name="nachname" value="$data[0]">\n  </div>\n  <br>\n  <div>\n   Dein Vorname:\n   <br>\n   <input name="vorname" value="$data[1]">\n  </div>\n  <br>\n  <div>\n   Dein Alter:\n   <br>\n   <input name="alter" value="$data[2]">\n  </div>\n  <br>\n  <div>\n   Dein Wohnort:\n   <br>\n   <input name="wohnort" value="$data[3]">\n  </div>\n  <br>\n  <div>\n   Deine E-Mail Adresse:\n   <br>\n   <input name="email" value="$data[4]">\n  </div>\n  <br>\n  <div>\n   Deine ICQ-Nummer:\n   <br>\n   <input name="icq" value="$data[5]">\n  </div>\n  <br>\n  <div>\n   Deine Homepage:\n   <br>\n   <input name="homepage" value="$data[6]">\n  </div>\n  <br>\n  <div>\n   Weiteres:\n   <br>\n   <textarea rows="5" cols="40"name="weiteres" wrap="virtual">$data[7]</textarea>\n  </div>\n];
    }
    print qq[  <hr>\n  <font color=ffffff>\n   <input type="hidden" name="request" value="divcom">\n   <input type="hidden" name="alias" value="$alias">\n   <input type="hidden" name="tmpid" value="$tmpid">\n   <input type="hidden" name="room" value="$room">\n   <input type="submit" value="Daten \344ndern ...">\n  </font>\n </form>\n</body>\n</html>\n];
}
sub edit {
    my $method = $_[0];
    if ($method eq 'edit_profile') {
        my $url = $homepage;
        $url =~ s[^http://*][];
        open PROFILE, ">data/user/profiles/$alias";
        my $profile = "$nachname\n$vorname\n$alter\n$wohnort\n$email\n$icq\nhttp://$url\n$weiteres";
        $profile =~ s/</&lt;/g;
        print PROFILE $profile;
        close PROFILE;
        &edit_html('edit_profile_html', 'edit_profile_ok');
    }
    elsif ($method eq 'edit_options') {
        if ($id ne $tmp) {
            &edit_html('edit_options_html', 'edit_pw_wrong');
            $client->shutdown(2);
            exit 0;
        }
        open OPTIONS, ">data/user/options/$alias";
        my $options = "$id\n$col\n$secol\n$maxVerlauf\n$verlaufOrder";
        $options =~ s/</&lt;/g;
        print OPTIONS $options;
        close OPTIONS;
        my $room = &get_room($alias);
        &private_javascript($alias, $room, "parent.input.document.inf.col.value = '$col'; parent.input.document.inf.secol.value = '$secol';");
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> <font color='$secol'>hat seine Chat-Einstellungen ge&auml;ndert</font> ...</font></i>", $alias);
        &post_strmsg($alias, $room, 'col', "$col;$secol");
        &edit_html('edit_options_html', 'edit_options_ok');
    }
}
1;
