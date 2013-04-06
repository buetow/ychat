sub command {
    $_[0] = $_[0] . ' ';
    $_[0] = substr($_[0], 1, length($_[0]) - 1);
    my(@cmd) = split(/ /, $_[0], 0);
    my $cmd = substr($_[0], 0, index($_[0], ' '));
    my $tmp = $_[1];
    sub lock {
        my $method = shift @_;
        open RSTAT, "data/online/rstat/$room";
        my(@r_stat) = <RSTAT>;
        close RSTAT;
        my $lock = "0\n";
        if ($room eq $standardroom) {
            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> sperrt den Raum ohne Erfolg ab ...</font></i>", $alias);
        }
        else {
            if ($r_stat[0] == 0) {
                $lock = "1\n";
                &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> sperrt den Raum ab ...</font></i>", $alias) if $method ne 'separee';
            }
            else {
                &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> sperrt den Raum auf ...</font></i>", $alias) if $method ne 'separee';
            }
            for ($i = 1; $i <= $#r_stat; ++$i) {
                $lock = $lock . $r_stat[$i];
            }
            open RSTAT, ">data/online/rstat/$room";
            print RSTAT $lock;
            close RSTAT;
        }
        &reload_html;
    }
    if ($cmd eq 'about') {
        &post_prv($alias, "<font color=ffffff>Aktuelle Version ist $title und wurde am $datum aktuallisiert. Programmiert von  Paul C. B\374tow  (2000 - 2002).</font>");
        &reload_html;
    }
    elsif ($cmd eq 'away') {
        for ($i = 1; $i <= $#cmd; ++$i) {
            $away_msg = $away_msg . ' ' . $cmd[$i];
        }
        my $room = &get_room($alias);
        if ($tmp eq 'autoaway') {
            $away_msg = "$alias wurde vom System agbemeldet ...";
            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='ffa500'><b>Das System</b></font> meldet <font color='".&get_ident($alias,3)."'>$alias</font> wegen Inaktivit&auml;t ab ...</i></font>", $alias);
        }
        else {
            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> meldet sich ab (<font color='$col'>$away_msg</font> ) ...</i></font>", $alias);
        }
        open AWAY, ">>data/online/rstat/$room.away";
        print AWAY "$alias<;$away_msg\n";
        close AWAY;
        &private_javascript($alias, $room, q[parent.input.document.inf.tmp.value = 'away'; parent.online.location.reload();]);
        &reload_html;
    }
    elsif ($cmd eq 'clear') {
        &post_prv($alias, "<script language='JavaScript'>window.open('chat?request=stream&alias=$alias&room=$room&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','main');</script>");
        &reload_html;
    }
    elsif ($cmd eq 'col') {
        my $col, $secol;
        if ($cmd[1] ne undef) {
            $col = $cmd[1];
            &prove_color($alias, $col);
            my $tmpsecol = $cmd[2];
            my $length = length $tmpsecol;
            if ($tmpsecol eq '' or $length < 6 or $length > 6) {
                $secol = $col;
            }
            else {
                $secol = $tmpsecol;
            }
        }
        else {
            $col = &random_color;
            $secol = &random_color;
        }
        open OPTIONS, "<data/user/options/$alias";
        my(@options) = <OPTIONS>;
        close OPTIONS;
        open OPTIONS, ">data/user/options/$alias";
        print OPTIONS $options[0] . $col . "\n" . $secol;
        close OPTIONS;
        open IDENT, "<data/online/ident/$alias";
        my(@ident) = <IDENT>;
        close IDENT;
        open IDENT, ">data/online/ident/$alias";
        print IDENT $ident[0] . $ident[1] . $ident[2] . "$col\n$secol";
        close IDENT;
        &post($room, '<i><font color=ffffff>' . &timestring . "</font> <font color=$col>$alias</font> <font color=$secol> hat die Farbe gewechselt...</font></i>", $alias);
        &private_javascript($alias, $room, "parent.input.document.inf.col.value = '$col'; parent.input.document.inf.secol.value = '$secol';");
        &post_strmsg($alias, $room, 'col', "$col;$secol");
        &reload_html;
    }
    elsif ($cmd eq 'date') {
        my($secs, $mins, $hours, $days, $months, $years) = &timearray;
        &post_prv($alias, "<font color=ffffff>Heute ist der $days.$months. Jahr $years. Es ist genau $hours:$mins Uhr und $secs Sekunden.</font>");
        &reload_html;
    }
    elsif ($cmd eq 'exit') {
        my $room = &get_room($alias);
        if ($tmp eq 'autologout') {
            &private_javascript($alias, $room, q[open('chat?request=start&tmp=autologout','_top');]);
            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='ffa500'><b>Das System</b></font> loggt <font color='".&get_ident($alias,3)."'>$alias</font> wegen Inaktivit&auml;t aus ... </font></i>", $alias);
        }
        else {
            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> loggt sich ganz aus ... </font></i>", $alias);
            &private_javascript($alias, $room, q[open('chat?request=start&tmp=quit','_top');]);
        }
        &rm_alias($alias, $room);
        &rm_away($alias, $room);
        &reload_html;
    }
    elsif ($cmd eq 'getcol') {
    	my $cols2show = $cmd[1];
        if (-f "data/user/options/$cols2show") {
        	open FILE, "<data/user/options/$cols2show";
                my(@file) = <FILE>;
                close FILE;
                chomp $file[1];
                chomp $file[2];
                &post_prv($alias, "<font color='ffa500'>$cols2show <font color='ffffff'> verwendet als 1. Farbe <font color='$file[1]'>$file[1]</font> und als 2. Farbe <font color='$file[2]'>$file[2]</font> !</font></font>");
                &reload_html;
         }
         &post_prv($alias, "<font color='ffa500'>$cols2show <font color='ffffff'> existiert nicht in der Benutzerdatenbank!</font></font>");
         &reload_html;
    }
    elsif ($cmd eq 'hits' or $cmd eq 'uptime') {
        &post_prv($alias, "<font color=ffffff>Uptime: $startime " . &calculate_uptime_hours($STARTIME) . "h; $HITS Hits; " . &calculate_hpm($HITS, $STARTIME) . ' Hits pro Minute</font>');
        &reload_html;
    }
    elsif ($cmd eq 'ig') {
        my $alias2ignore = $cmd[1];
        if ($alias2ignore) {
            if (-f "data/user/options/$alias2ignore") {
		if (3 <= &hierachie($alias2ignore)) {
        	        &post_prv($alias, "<font color='ffa500'>$alias2ignore</font> <font color='ffffff'> kann nicht ignoriert werden!</font>");
	    	}
	    	else {
                	&post_strmsg($alias, $room, 'ig', $alias2ignore);
                }
            }
            else {
                &post_prv($alias, "<font color='ffa500'>$alias2ignore</font> <font color='ffffff'>existiert nicht in der Datenbank!</font>");
            }
        }
        else {
            &post_strmsg($alias, $room, 'igl');
        }
        &reload_html;
    }
    elsif ($cmd eq 'j' or $cmd eq 'sep') {
        if ($cmd[1] =~ /\W/ or $cmd[1] eq '') {
            &post_prv($alias, "<font color=ffffff>Der Raum <font color=ffa500>$cmd[1]</font> enth\344lt illegale Zeichen!</font>");
            &reload_html;
        }
        else {
            if ($cmd eq 'j') {
                &change_room('j', $cmd[1], $room);
                &reload_html;
            }
            else {
                if ($cmd[1] eq $standardroom) {
                    &post_prv($alias, "<font color=ffffff>Der Raum <font color=ffa500>$cmd[1]</font> ist ein Standardraum und kann nicht als Separee er&ouml;ffnet werden!</font>");
                }
                else {
                    &change_room('sep', $cmd[1], $room);
                    &lock('separee');
                }
                &reload_html;
            }
        }
    }
    elsif ($cmd eq 'me' or $cmd eq 'm') {
        my $me_msg;
        for ($i = 1; $i <= $#cmd; ++$i) {
            $me_msg = $me_msg . ' ' . $cmd[$i];
        }
        $me_msg =~ s[/me/][<i>$alias</i>]g;
        $me_msg =~ s[/m/][<i><font color=$col>$alias</font></i>]g;
        if ($cmd eq 'm') {
            &post($room, "<b><font color=$col><i>$alias</i> <font color=$secol>$me_msg</font></font></b>", $alias);
        }
        else {
            &post($room, "<b><font color=$col><i>$alias</i> $me_msg</font></b>", $alias);
        }
        &reload_html;
    }
    elsif ($cmd eq 'memo') {
        my $alias2memo = $cmd[1];
        my($secs, $mins, $hours, $days, $months, $years) = &timearray;
        if (-f "data/user/options/$alias2memo") {
            my $memo_msg;
            for ($i = 2; $i <= $#cmd; ++$i) {
                $memo_msg = $memo_msg . ' ' . $cmd[$i];
            }
            &post_prv($alias, "<font color=ffa500>$alias2memo <font color=ffffff>wurde folgende memo erfolgreich hinterlassen: $memo_msg</font></font>");
            open MEMO, ">>data/user/memos/$alias2memo";
            print MEMO "<font color=ffffff>($days.$months.$years - $hours:$mins:$secs) <font color=$col>$alias</font> hat Dir eine Memo hinterlassen: <font color=ffa500>$memo_msg</font></font><br>\n";
            close MEMO;
            &reload_html;
        }
        &post_prv($alias, "<font color=ffa500>$alias2memo <font color=ffffff>existiert in der Datenbank nicht !</font></font>");
        &reload_html;
    }
    elsif ($cmd eq 'menu') {
        &private_javascript($alias, $room, "window.open('chat?request=htdocs&alias=$alias&var=menu_html&room=$standardroom&col=$col&secol=$secol&tmpid=$tmpid','_new','width=500,height=500,screenX=100,screenY=100,scrollbars');");
        &reload_html;
    }
    elsif ($cmd eq 'online') {
        my $alias2prove = $cmd[1];
	my $totaltime = &get_online_time($alias2prove);
        if ($totaltime ne false) {
            &post_prv("$alias", "<font color=ffa500>$alias2prove <font color=ffffff>war insgesamt schon $totaltime Stunden online!</font></font>");
        }
        else {
            &post_prv("$alias", "<font color=ffa500>$alias2prove <font color=ffffff>existiert nicht in der Benutzerdatenbank!</font></font>");
        }
        &reload_html;
    }
    elsif ($cmd eq 'q') {
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> verl&auml;sst den Chat ... </font></i>", $alias);
        &post_strmsg($alias, $room, 'quit');
        unlink "data/online/rooms/$room/$alias";
        &rm_rstat($alias, $room);
        &rm_away($alias, $room);
        &check_online;
        &reload_html;
    }
    elsif ($cmd eq 'rmgb') {
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> l\366scht das eigene G\344stebuch  ...</font></i>", $alias);
        unlink "data/user/gbooks/$alias";
        &reload_html;
    }
    elsif ($cmd eq 's') {
        my $msg2scream;
        for ($i = 1; $i <= $#cmd; ++$i) {
            $msg2scream = $msg2scream . ' ' . $cmd[$i];
        }
        $msg2scream =~ s/\344/\304/g;
        $msg2scream =~ s/\366/\326/g;
        $msg2scream =~ s/\374/\334/g;
        $msg2scream = "\U$msg2scream\E";
        &post($room, "<b><font color=$col><i>$alias schreit:</i> <font color=$secol>$msg2scream</font></font></b>", $alias);
        &reload_html;
    }
    elsif ($cmd eq 'seen') {
        my $alias2prove = $cmd[1];
        if (-f "data/user/options/$alias2prove") {
            if (-f "data/user/lastlogin/$alias2prove") {
                open LASTLOGIN, "<data/user/lastlogin/$alias2prove";
                my $lastlogin = <LASTLOGIN>;
                close LASTLOGIN;
                my $timestamp = sprintf('%.2f', (time - $lastlogin) / 3600);
                &post_prv("$alias", "<font color='ffa500'>$alias2prove <font color=ffffff> hat sich vor $timestamp Stunden zuletzt in das System eingeloggt!</font></font>");
            }
            else {
                &post_prv("$alias", "<font color='ffa500'>$alias2prove <font color=ffffff> hat sich seit der Einf\374hrung dieses Befehls noch nie eingeloggt!</font></font>");
            }
        }
        else {
            &post_prv("$alias", "<font color='ffa500'>$alias2prove <font color=ffffff> exsistiert nicht in der Benutzerdatenbank!</font></font>");
        }
        &reload_html;
    }
    elsif ($cmd eq 'selfkill') {
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> entfernt sich ganz und gar aus dem System und loggt sich aus ... </font></i>", $alias);
        &private_javascript($alias, $room, q[open('chat?request=start&tmp=selfkill','_top');]);
        &kill_alias($alias);
        &reload_html;
    }
    elsif ($cmd eq 'w') {
        my $theme = &get_theme($room);
        my $door = '[<font color=red>Abgesperrt!</font>]' if $rstat[0] eq "1\n";
        my($is, $nr) = ('t', 0);
        my $persons;
        opendir DIR, "data/online/rooms/$room";
        my(@dir) = readdir DIR;
        closedir DIR;
        foreach $_ (@dir) {
            if (-f "data/online/rooms/$room/$_") {
                $persons = $persons . ' ' . "<font color='".&get_ident($_,3)."'>$_</font>";
                ++$nr;
            }
        }
        &post_prv($alias, "<font color=ffffff>Der aktuelle <b>Raum:</b> $room $theme $door ($nr): $persons</font>");
        &reload_html;
    }
    elsif ($cmd eq 'wc' or $cmd eq 'wc/') {
        my $raum = $cmd[1];
        my $persons;
        if ($raum eq '' and $cmd eq 'wc') {
            opendir USERS, 'data/online/users';
            my(@users) = readdir USERS;
            close USERS;
            foreach $_ (@users) {
                $persons = $persons . ' ' . "<font color='".&get_ident($_,3)."'>$_</font>" if -f "data/online/users/$_";
            }
            my $nr = $#users - 1;
            my($nrs, $is) = ('', 'ist');
            if ($nr != 1) {
                $nrs = 'en';
                $is = 'sind';
            }
            &post_prv("$alias", "<font color=ffffff>$nr Person$nrs $is insgesamt eingeloggt: $persons</font>");
        }
        elsif ($raum eq '' and $cmd eq 'wc/') {
            my $msg2post;
            opendir DIR, 'data/online/rooms';
            my(@dir) = readdir DIR;
            closedir DIR;
            foreach $_ (@dir) {
                if (-e "data/online/rooms/$_" and $_ ne '..' and $_ ne '.') {
                    opendir DIR, "data/online/rooms/$_";
                    my(@dir2) = readdir DIR;
                    closedir DIR;
                    open RSTAT, "<data/online/rstat/$_";
                    my(@rstat) = <RSTAT>;
                    close RSTAT;
                    chomp $rstat[1];
                    my $door = '[<font color=red>Abgesperrt!</font>]' if $rstat[0] eq "1\n";
                    my $nr = $#dir2 - 1;
                    $msg2post = $msg2post . "<tr><td><b><font size=2 color='ffffff'>$_ $door ($nr) [" . $rstat[1] . q(]: </font></b></td></tr><tr><td><font size=2 color='ffa500'>);
                    foreach $chatter (@dir2) {
                        $msg2post = $msg2post . "<font color='".&get_ident($chatter,3)."'>$chatter</font> " if -f "data/online/rooms/$_/$chatter";
                    }
                    print '</font></td>';
                }
            }
            &post_prv($alias, "<font color=ffffff>Folgende R&auml;ume sind offen:</font><table>$msg2post</table>");
        }
        elsif ($raum ne '') {
            my $persons;
            opendir DIR, "data/online/rooms/$raum";
            my(@dir) = readdir DIR;
            closedir DIR;
            foreach $_ (@dir) {
                $persons = $persons . ' ' . "<font color='".&get_ident($chatter,3)."'>$_</font>" if -f "data/online/rooms/$raum/$_";
            }
            my($nr, $is) = ($#dir - 1, 't');
            if ($nr != 1) {
                $nrs = 'en';
                $is = 'n';
            }
            &post_prv($alias, "<font color=ffffff> $nr Person$nrs befinde$is sich im Raum <b>$raum</b> $persons</font>");
        }
        &reload_html;
    }
    elsif ($cmd eq 'wj' or $cmd eq 'ju') {
        my $alias2search = $cmd[1];
        my $roomofalias = &get_room($alias2search);
        if ($roomofalias) {
            if ($cmd eq 'wj') {
                &post_prv($alias, "<font color='ffa500'>$alias2search <font color=ffffff>befindet sich im Raum <b>$roomofalias</b> !</font></font>");
            }
            else {
                &change_room('ju', $roomofalias, $room, $alias2search);
            }
        }
        else {
            &post_prv($alias, "<font color='ffa500'>$alias2search <font color=ffffff> ist nicht eingeloggt!</font></font>");
        }
        &reload_html;
    }
    elsif (1 <= &hierachie($alias, $room)) {
        if ($cmd eq 'l') {
            &lock;
        }
        elsif ($cmd eq 'su') {
            my $alias2su = $cmd[1];
            opendir DIR, "data/online/rooms/$room";
            my(@dir) = readdir DIR;
            closedir DIR;
            foreach $_ (@dir) {
                if ($_ eq $alias2su) {
                    open DATEI, "<data/online/rstat/$room";
                    my(@rstat) = <DATEI>;
                    close DATEI;
                    if (1 <= hierachie($alias2su, $room)) {
                        &post_prv($alias, "<font color='ffa500'>$alias2su</font> <font color='ffffff'> kann nicht zum Raumbesetzer ernannt werden. (Ist bereits RB, W\304 oder OW)</font>");
                        &reload_html;
                    }
                    open RSTAT, ">>data/online/rstat/$room";
                    print RSTAT "$alias2su<;1\n";
                    close RSTAT;
                    &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> verleiht <font color='".&get_ident($alias2su,3)."'>$alias2su</font> Raumbesetzerrechte ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                    &reload_html;
                }
            }
            &post_prv($alias, "<font color='ffa500'>$alias2su</font> <font color=ffffff> kann nicht zum Raumbesetzer ernannt werden. (Ist nicht anwesend)</font>");
            &reload_html;
        }
        elsif ($cmd eq 'rsu') {
            my $alias2rsu = $cmd[1];
            if (1 == hierachie($alias2rsu, $room)) {
                &rm_rstat($alias2rsu, $room);
                &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> entzieht <font color='".&get_ident($alias2rsu,3)."'>$alias2rsu</font> die Raumbesetzerrechte ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
            }
            else {
                &post_prv($alias, "<font color='ffa500'>$alias2rsu <font color='ffffff'>ist gar kein Raumbesetzer!</font></font>");
            }
            &reload_html;
        }
        elsif ($cmd eq 't') {
            open RSTAT, "data/online/rstat/$room";
            my(@r_stat) = <RSTAT>;
            close RSTAT;
            my $theme;
            if ($cmd[1] ne undef) {
                for ($i = 1; $i <= $#cmd; ++$i) {
                    $theme = $theme . ' ' . $cmd[$i];
                }
                &post($room, q[<i><font color='ffffff'>] . &timestring . qq[ <font color='$col'>$alias</font> gibt dem Raum das Thema "<font color='$col'>$theme</font>" ...</font></i>], $alias);
            }
            else {
                &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> l\366scht das Thema des Raums ...</font></i>", $alias);
            }
            $theme = $r_stat[0] . $theme . "\n";
            for ($i = 2; $i <= $#r_stat; ++$i) {
                $theme = $theme . $r_stat[$i] . "\n";
            }
            open RSTAT, ">data/online/rstat/$room";
            print RSTAT $theme;
            close RSTAT;
            &reload_html;
        }
        elsif (3 <= &hierachie($alias, $room)) {
            if ($cmd eq 'ban' or $cmd eq 'banip') {
                my $alias2ban = $cmd[1];
                my $hierachie = &hierachie($alias2ban);
                open BAN, '<data/ban';
                my(@ban) = <BAN>;
                close BAN;
                if ($cmd[1] eq undef) {
                    my $nr = @ban;
                    my $banned;
                    foreach $_ (@ban) {
                        chomp $_;
                        $banned .= " $_";
                    }
                    &post_prv($alias, "<font color=ffffff>Folgende Benutzer sind verbannt [$nr]: <font color='ff0000'>$banned</font></font>");
                    &reload_html;
                }
                elsif (-f "data/user/options/$alias2ban") {
                    if (3 <= $hierachie) {
                        &post_prv($alias, "<font color='ffa500'>$alias2ban <font color='ffffff'>kann nicht verbannt werden!</font></font>");
                        &reload_html;
                    }
                    if ($cmd eq 'ban') {
                        if (&add_entry($alias2ban, 'ban') == 1) {
                            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> verbannt <b>$alias2ban</b> aus dem Chat ...</font></i>", $alias);
                        }
                        else {
                            &post_prv($alias, "<font color='ffa500'>$alias2ban <font color=ffffff>ist bereits verbannt!</font></font>");
                        }
                    }
                    elsif ($cmd eq 'banip') {
                        open DATEI, "<data/online/ident/$alias2ban";
                        my $ipnr = <DATEI>;
                        close DATEI;
                        chomp $ipnr;
                        if (add_entry("ip://$ipnr", 'ban') == 1) {
                            &post_prv($alias, "<font color='ffffff'>IP-Nummer von <font color='ffa500'>$alias2ban</font> (<font color='ff0000'>$ipnr</font>) wurde verbannt!</font></i>");
                        }
                        else {
                            &post_prv($alias, "<font color='ffffff'>Die IP-Nummer <font color='ff0000'>$ipnr</font> von <font color='ffa500'>$alias2ban</font> ist bereits verbannt!</font></font>");
                        }
                        &reload_html;
                    }
                    &reload_html;
                }
                &post_prv($alias, "<font color='ffa500'>$alias2ban <font color=ffffff> kann nicht verbannt werden, da Benutzername nicht existiert!</font></font>");
                &reload_html;
            }
            elsif ($cmd eq 'rban' or $cmd eq 'rbanip') {
                my $alias2rban = $cmd[1];
                open BAN, '<data/ban';
                @ban = <BAN>;
                close BAN;
                foreach $ban (@ban) {
                    if (&rm_entry($alias2rban, 'ban') == 1) {
                        if ($alias2rban =~ /ip:/) {
                            &post_prv($alias, "<font color='ffffff'>Die IP-Nummer <font color='ff0000'>$alias2rban</font> wurde aus der Banliste ausgetragen!</font></font>");
                        }
                        else {
                            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> entbannt <font color='".&get_ident($alias2rban,3)."'>$alias2rban</font> ...</font></i>", $alias);
                        }
                        &reload_html;
                    }
                }
                &post_prv($alias, "<font color='ff0000'>$alias2rban <font color='ffffff'>ist nicht verbannt!</font></font>");
                &reload_html;
            }
            elsif ($cmd eq 'img') {
                my $hierachie = &hierachie($alias, $room);
                &hierachie_rm($alias);
                if ($hierachie == 3) {
                    &post_prv($alias, q[<font color='ffffff'>Als tempor&auml;rer W&auml;chter kannst du dein Superuserlogo nicht verstecken!</font>]);
		}
		elsif ($hierachie % 2 > 0) {
                    &post_prv($alias, q[<font color='ffffff'>Dein Superuserlogo wurde versteckt!</font>]);
                    &hierachie_add($alias, $hierachie + 1);
                }
                else {
                    &post_prv("$alias", q[<font color='ffffff'>Dein Superuserlogo wurde wieder sichtbar gemacht!</font></font>]);
                    &hierachie_add($alias, $hierachie - 1);
                }
                &reload_html;
            }
            elsif ($cmd eq 'ip') {
                my $alias2prove = $cmd[1];
                opendir DIR, 'data/online/ident';
                my(@dir) = readdir DIR;
                closedir DIR;
                foreach $_ (@dir) {
                    if ($_ eq $alias2prove) {
                        open DATEI, "<data/online/ident/$alias2prove";
                        my $ipnr = <DATEI>;
                        close DATEI;
                        chomp $ipnr;
                        &post_prv($alias, "<font color='ffa500'>$_<font color='ffffff'> hat die IP-Nummer <font color='ff0000'>$ipnr</font> !</font></font>");
                    }
                }
                &reload_html;
            }
            elsif ($cmd eq 'k' or $cmd eq 'o') {
                my $alias2kick = $cmd[1];
                if (-f "data/online/users/$alias2kick") {
                    opendir DIR, 'data/online/rooms';
                    my(@dir) = readdir DIR;
                    closedir DIR;
                    foreach my $raum (@dir) {
                        opendir DIR, "data/online/rooms/$raum";
                        my(@dir2) = readdir DIR;
                        closedir DIR;
                        foreach my $chatter (@dir2) {
                            if ($chatter eq $alias2kick) {
                                my $hierachie2 = &hierachie($alias2kick);
                                if (&hierachie($alias) <= &hierachie($alias2kick)) {
                                    &post_prv($alias, "<font color='ffa500'>$alias2kick <font color='ffffff'> besitzt einen h&ouml;heren oder gleichen Rang und kann nicht gekickt/ausgeloggt werden!</font></font>");
                                }
                                else {
                                    if ($cmd eq 'k') {
                                        &post_prv($alias2kick, '<script language="JavaScript">window.open("chat?request=start&tmp=kick","_top");</script>');
                                    }
                                    else {
                                        &post_prv($alias2kick, '<script language="JavaScript">window.open("chat?request=start&tmp=logout","_top");</script>');
                                    }
                                    $_ = undef if $room eq $raum;
                                    if ($cmd eq 'k') {
                                        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> katapultiert <font color='".&get_ident($alias2kick,3)."'>$alias2kick</font> aus dem Chat ...</font></i>", $alias, $raum);
                                    }
                                    else {
                                        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> loggt <font color='".&get_ident($alias2kick,3)."'>$alias2kick</font> aus dem Chat aus ...</font></i>", $alias, $alias);
                                    }
                                    &rm_alias($alias2kick, $raum);
                                    &check_online;
                                }
                                &reload_html;
                            }
                        }
                    }
                }
                &post_prv($alias, "<font color='ffa500'>$alias2kick <font color='ffffff'> ist nicht anwesend und kann nicht aus dem System katapultiert werden werden!</font></font>");
                &reload_html;
            }
            elsif ($cmd eq 'sl') {
                my $sl_room = $cmd[1];
                if (-e "data/online/rooms/$sl_room") {
                    my $lock = "0\n";
                    open RSTAT, "<data/online/rstat/$sl_room";
                    my(@r_stat) = <RSTAT>;
                    close RSTAT;
                    if ($r_stat[0] == 0) {
                        $lock = "1\n";
                        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> sperrt den Raum <b>$sl_room</b> von aussen ab ...</font></i>", $alias, $sl_room);
                    }
                    else {
                        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> sperrt den Raum <b>$sl_room</b> von aussen auf ...</font></i>", $alias, $sl_room);
                    }
                    for ($i = 1; $i <= $#r_stat; ++$i) {
                        $lock = $lock . $r_stat[$i];
                    }
                    open RSTAT, ">data/online/rstat/$sl_room";
                    print RSTAT $lock;
                    close RSTAT;
                }
                else {
                    &post_prv($alias, "<font color='ffffff'>Der Raum <font color='ffa500'>$sl_room</font> existiert nicht!</font></font>");
                }
                &reload_html;
            }
            elsif ($cmd eq 'ss') {
                my $ss_room = $cmd[1];
                if (-e "data/online/rooms/$ss_room") {
                    my $ss_msg;
                    for ($i = 2; $i <= $#cmd; ++$i) {
                        $ss_msg = $ss_msg . ' ' . $cmd[$i];
                    }
                    my $msg2scream = "\U$ss_msg\E";
                    &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> ruft in den Raum <b>$ss_room</b> hinein: <b><font color=$col>$msg2scream</font></b> ...</font></i>", $alias, $ss_room);
                    &reload_html;
                }
                else {
                    &post_prv($alias, "<font color='ffffff'>Der Raum <font color='ffa500'>$ss_room</font> existiert nicht!</font></font>");
                }
                &reload_html;
            }
            elsif ($cmd eq 'st') {
                my $st_room = $cmd[1];
                if (-e "data/online/rooms/$st_room") {
                    my $theme;
                    if ($cmd[2] ne undef) {
                        for ($i = 1; $i <= $#cmd; ++$i) {
                            $theme = $theme . ' ' . $cmd[$i];
                        }
                        &post($room, '<i><font color=ffffff>' . &timestring . qq[ <font color='$col'>$alias</font> gibt dem Raum <b>$st_room</b> das Thema "<font color='$col'>$theme</font>" ...</font></i>], $alias, $st_room);
                    }
                    else {
                        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> l\366scht das Thema des Raums <b>$st_room</b> ...</font></i>", $alias, $st_room);
                    }
                    open RSTAT, "<data/online/rstat/$st_room";
                    my(@r_stat) = <RSTAT>;
                    close RSTAT;
                    $theme = $r_stat[0] . $theme . "\n";
                    for ($i = 2; $i <= $#r_stat; ++$i) {
                        $theme = $theme . $r_stat[$i];
                    }
                    open RSTAT, ">data/online/rstat/$st_room";
                    print RSTAT $theme;
                    close RSTAT;
                }
                else {
                    &post_prv($alias, "<font color=ffffff>Der Raum <font color=ffa500>$st_room</font> existiert nicht!</font></font>");
                }
                &reload_html;
            }
            elsif ($cmd eq 'status') {
                my $alias2level = $cmd[1];
                &post_prv($alias, "<font color='ffa500'>$alias2level <font color='ffffff'>hat den Superuserstatus <font color='ff0000'>".&hierachie($alias2level, &get_room($alias2level))."</font> !</font></font>");
                &reload_html;
            }
            elsif (7 <= &hierachie($alias)) {
                if ($cmd eq 'kill') {
                    my $alias2kill = $cmd[1];
                    if (-f "data/user/options/$alias2kill") {
                        if (7 <= &hierachie($alias2kill)) {
                            &post_prv($alias, "<font color='ffa500'>$alias2kill</font> <font color='ffffff'>hat einen h&ouml;heren Rang und kann nicht aus dem System entfernt werden !</font>");
                        }
                        else {
                            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col>$alias</font> killt <b>$alias2kill</b> ganz und gar aus dem System ...</font></i>", $alias);
                            &kill_alias($alias2kill, $alias);
                        }
                    }
                    else {
                        &post_prv($alias, "<font color='ffa500'>$alias2kill <font color='ffffff'> existiert nicht in der Datenbank!</font></font>");
                    }
                    &reload_html;
                }
                elsif ($cmd eq 'sys') {
                    my $sys_msg;
                    for ($i = 1; $i <= $#cmd; ++$i) {
                        $sys_msg = $sys_msg . ' ' . $cmd[$i];
                    }
                    $sys_msg = q[<i><font color='ffffff'>] . &timestring . " </font></i> <b><font size=2 color='red'>SYSTEMNACHRICHT</font></b><i><font color='ffffff'>: $sys_msg ...</font></i>";
                    opendir ROOMS, 'data/msgs';
                    my(@rooms) = readdir ROOMS;
                    close ROOMS;
                    foreach $_ (@rooms) {
                        &post($_, $sys_msg, $alias) unless /\./;
                    }
                    &log($sys_msg);
                    &reload_html;
                }
                elsif ($cmd eq 'twa') {
                    my $alias2wa = $cmd[1];
                    if (-f "data/user/options/$alias2wa") {
			my $room2 = &get_room($alias2wa);
			if (3 <= &hierachie($alias2wa, $room2)) {
                            &post_prv($alias, "<font color='ffa500'>$alias2wa <font color='ffffff'>kann nicht zum tempor&auml;ren W&auml;chter ernannt werden!</font></font>");
                        }
                        else {
		            &hierachie_add_rstat($alias2wa, 3, $room2);
                            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> ernennt <font color='".&get_ident($alias2wa,3)."'>$alias2wa</font> zum <font color='ff0000'>tempor&auml;ren</font> W\344chter ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                        }
                        &reload_html;
                    }
                    else {
                        &post_prv($alias, "<font color='ffa500'>$alias2wa <font color=ffffff>existiert nicht in der Datenbank!</font></font>");
                    }
                    &reload_html;
                }
                elsif ($cmd eq 'rtwa') {
                    my $alias2rwa = $cmd[1];
		    my $room2 = &get_room($alias2rwa);
                    if (3 == &hierachie($alias2rwa, $room2)) {
                        &hierachie_rm_rstat($alias2rwa, $room2);

			&post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> hat <font color='".&get_ident($alias2rwa,3)."'>$alias2rwa</font> vom <font color='ff0000'>tempor&auml;ren</font> W\344chter zum normalen Chatter degradiert ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                    }
                    else {
                        &post_prv($alias, "<font color='ffa500'>$alias2rwa <font color=ffffff>ist kein tempor&auml;rer W\344chter und k\366nnen diese Rechte nicht entzogen werden!</font></font>");
                    }
                    &reload_html;
                }
                elsif ($cmd eq 'wa') {
                    my $alias2wa = $cmd[1];
                    if (-f "data/user/options/$alias2wa") {
                        if (5 <= &hierachie($alias2wa)) {
                            &post_prv($alias, "<font color='ffa500'>$alias2wa <font color='ffffff'>kann nicht zum W&auml;chter ernannt werden!</font></font>");
                        }
                        else {
                            &hierachie_add($alias2wa, 5);
                            &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> ernennt <font color='".&get_ident($alias2wa,3)."'>$alias2wa</font> zum W\344chter ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                        }
                        &reload_html;
                    }
                    else {
                        &post_prv($alias, "<font color='ffa500'>$alias2wa <font color=ffffff>existiert nicht in der Datenbank!</font></font>");
                    }
                    &reload_html;
                }
                elsif ($cmd eq 'rwa') {
                    my $alias2rwa = $cmd[1];
                    my $hierachie = &hierachie($alias2rwa);
                    if (5 == $hierachie or 6 == $hierachie) {
                        &hierachie_rm($alias2rwa);
                        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> hat <font color='".&get_ident($alias2rwa,3)."'>$alias2rwa</font> vom W\344chter zum normalen Chatter degradiert ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                    }
                    else {
                        &post_prv($alias, "<font color='ffa500'>$alias2rwa <font color=ffffff>ist kein W\344chter und k\366nnen diese Rechte nicht entzogen werden!</font></font>");
                    }
                    &reload_html;
                }
                elsif (9 <= &hierachie($alias)) {
                    if ($cmd eq 'ow') {
                        my $alias2ow = $cmd[1];
                        if (-f "data/user/options/$alias2ow") {
                            if (7 <= &hierachie($alias2ow)) {
                                &post_prv($alias, "<font color='ffa500'>$alias2ow <font color='ffffff'>kann nicht zum Oberw&auml;chter ernannt werden!</font></font>");
                            }
                            else {
                                &hierachie_add($alias2ow, 7);
                                &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> ernennt <font color='".&get_ident($alias2ow,3)."'>$alias2ow</font> zum Oberw&auml;chter ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                            }
                            &reload_html;
                        }
                        else {
                            &post_prv($alias, "<font color='ffa500'>$alias2wa <font color=ffffff>existiert nicht in der Datenbank!</font></font>");
                        }
                        &reload_html;
                    }
                    elsif ($cmd eq 'row') {
                        my $alias2row = $cmd[1];
                        my $hierachie = &hierachie($alias2row);
                        if (7 == $hierachie or 8 == $hierachie) {
                            &hierachie_rm($alias2row);
                            &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> hat <font color='".&get_ident($alias2row,3)."'>$alias2row</font> vom Oberw\344chter zum normalen Chatter degradiert ...</font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
                        }
                        else {
                            &post_prv($alias, "<font color='ffa500'>$alias2row <font color=ffffff>ist kein Oberw\344chter und k\366nnen diese Rechte nicht entzogen werden!</font></font>");
                        }
                        &reload_html;
                    }
                    elsif ($cmd eq 'pw') {
                        my $pw2show = $cmd[1];
                        if (-f "data/user/options/$pw2show") {
                            open PW, "<data/user/options/$pw2show";
                            my(@pw) = <PW>;
                            close PW;
                            chomp $pw[0];
                            &post_prv($alias, "<font color='ffa500'>$pw2show <font color='ffffff'> hat das Passwort <font color='ff0000'>$pw[0]</font> !</font></font>");
                            &reload_html;
                        }
                        &post_prv($alias, "<font color='ffa500'>$pw2show <font color='ffffff'> existiert nicht in der Benutzerdatenbank!</font></font>");
                        &reload_html;
                    }
                    elsif ($cmd eq 'id' or $cmd eq 'agent') {
                        my $id2show = $cmd[1];
                        if (-f "data/online/ident/$id2show") {
                            if ($cmd eq 'id') {
                                &post_prv($alias, "<font color='ffa500'>$id2show <font color='ffffff'> hat die Temp-ID <font color='ff0000'>".&get_ident($id2show, 1)."</font> !</font></font>");
                            }
                            else {
                                &post_prv($alias, "<font color='ffa500'>$id2show <font color='ffffff'> verwendet <font color='ff0000'>".&get_ident($id2show, 2)."</font> !</font></font>");
                            }
                            &reload_html;
                        }
                        &post_prv($alias, "<font color='ffa500'>$id2show <font color='ffffff'> ist nicht eingeloggt !</font></font>");
                        &reload_html;
                    }
                    elsif ($cmd eq 'cool') {
                        for ($i = 1; $i <= $#cmd; ++$i) {
                            $msg2post = $msg2post . ' ' . $cmd[$i];
                        }
                        &post($room, '<i><font color=ffffff>' . &timestring . " $msg2post</font></i>", $alias);
                        &reload_html;
                    }
                    elsif ($cmd eq 'rev') {
                        for ($i = 1; $i <= $#cmd; ++$i) {
                            $msg2post = $msg2post . ' ' . $cmd[$i];
                        }
                        $msg2post = reverse($msg2post);
                        &post($room, "<b><font color='$col'>$alias: <font color='$secol'>$msg2post</font></font></b>", $alias);
                        &reload_html;
                    }
                    elsif ($cmd eq 'arks') {
                        for ($i = 1; $i <= $#cmd; ++$i) {
                            $msg2post = $msg2post . ' ' . $cmd[$i];
                        }
                        $msg2post =~ s/i/1/gi;
                        $msg2post =~ s/e/3/gi;
                        $msg2post =~ s/a/4/gi;
                        $msg2post =~ s/s/5/gi;
                        $msg2post =~ s/g/6/gi;
                        $msg2post =~ s/t/7/gi;
                        $msg2post =~ s/b/8/gi;
                        $msg2post =~ s/o/0/gi;
			&post($room, "<b><font color='$col'>$alias: <font color='$secol'>$msg2post</font></font></b>", $alias);
                        &reload_html;
                    }
                    elsif ($cmd eq 'ultra') {
                        for ($i = 1; $i <= $#cmd; ++$i) {
                            $msg2post = $msg2post . ' ' . $cmd[$i];
                        }
			my @letters = split '', "$alias: $msg2post";
			$msg2post = undef;
			foreach (@letters) {
				$msg2post .= "<font color='".&random_color."'>$_</font>";
			}
			&post($room, "<b>$msg2post</b>", $alias);
                        &reload_html;
                    }
                }
            }
        }
    }
    my($alias2post, $msg2post, $action);
    if ($cmd eq 'msg') {
        $alias2post = $cmd[1];
        for ($i = 2; $i <= $#cmd; ++$i) {
            $msg2post = $msg2post . ' ' . $cmd[$i];
        }
    }
    elsif ($cmd eq '') {
        $alias2post = $whisper;
        for ($i = 1; $i <= $#cmd; ++$i) {
            $msg2post = $msg2post . ' ' . $cmd[$i];
        }
    }
    else {
        $alias2post = $cmd;
        for ($i = 1; $i <= $#cmd; ++$i) {
            $msg2post = $msg2post . ' ' . $cmd[$i];
        }
    }
    $msg2post =~ s[/me/][<i>$alias</i>]g;
    $msg2post =~ s[/m/][<i><font color=$col>$alias</font></i>]g;
    if (-f "data/online/users/$alias2post" and not -f "data/online/rooms/$loginplace/$alias2post") {
        &post_prv($alias, "<b><font color='$col'><i>Zu $alias2post gefl\374stert:</i> <font color='$secol'>$msg2post</font></font></i></b>");
        &post_prv($alias2post, "<b><font color='$col'><i>$alias fl\374stert:</i> <font color='$secol'>$msg2post</font></font></i></b>");
        &private_javascript($alias, $room, "parent.input.document.inf.whisper.value = '$alias2post';");
        &reload_html;
    }
    &post_prv($alias, qq[ <font color=ffffff>Benutzer oder Befehl <font color=ffa500>$alias2post</font> ist nicht anwesend/verf\374gbar! Bitte den Befehl "/menu" eingeben, um das Chat-Men&uuml zu &ouml;ffnen.</font></font>]);
    &reload_html;
}
1;
