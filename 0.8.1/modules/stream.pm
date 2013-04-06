sub stream {
    unlink "data/online/rooms/$loginplace/$alias";
    &secure_checkid($alias, $tmpid);
    my $pid = $$;
    $SIG{'CHLD'} = 'IGNORE';
    ++$|;
    print qq[<html>\n <head>\n<script language="JavaScript">\n<!--\n open('chat?request=online&room=$room&alias=$alias&tmpid=$tmpid','online');\n open('chat?request=input&alias=$alias&room=$room&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','input');\n\n// open('chat?request=input&alias=$alias&room=$room&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','input');\n\n// Scroll-Routine von Anklepper (http://www.anklepper.de)\n// speziell f\374r yChat (http://www.yChat.de), Copyright 2000\nscrolling      = true;\nmoves_steps    = 4;\nmoves_interval = 4;\nfunction moves()\n{\n  if( scrolling != false )\n   window.scrollBy( 0, moves_steps );\n  window.setTimeout( "moves()", moves_interval );\n}\nmoves();\n//-->\n</script>\n</head>\n<body bgcolor="$chatbg">\n];
    &check_online;
    print "<font size=1 color=ffffff face=arial>$title - Letzte &Auml;nderung $datum\n; Der Chat wird geladen...\n<br>";
    sleep 1;
    print "<b>Hallo <font color=$col>$alias</font></b>, mit dem Befehl /menu kannst du das Chat-Men&uuml; aufrufen!<br>";
    sleep 1;
    print "Du befindest dich im Raum <b>$room</b>: " . get_theme($room) . "<br>\n";
    print "</font>\n";

    if (-f "data/user/memos/$alias") {
        open MEMOS, "<data/user/memos/$alias";
        my(@memos) = <MEMOS>;
        close MEMOS;
        my $memos;
        foreach $_ (@memos) {
            $memos .= $_;
        }
        print $memos;
        unlink "data/user/memos/$alias";
    }

    my ($change_room,
    	$startime,
    	$flooding,
    	@ignore,
    	@flood);

    OPENMSGFILE:
    $change_room = 0;
    $startime = time;
    $flooding = 0;
    unless (-f "data/online/rstat/$room") {
        open DATEI, ">data/online/rstat/$room";
        print DATEI "0\n\n$alias<;1\n";
        close DATEI;
    }
    open DATEI, ">data/online/users/$alias";
    print DATEI time;
    close DATEI;
    open DATEI, ">data/online/rooms/$room/$alias";
    print DATEI time;
    close DATEI;
    if (-e "data/msgs/$room") {
        open MSGFILE, "<data/msgs/$room";
        mkdir 'data/online/pids', 511 unless -d 'data/online/pids';
        mkdir "data/online/pids/$room", 511 unless -d "data/online/pids/$room";
        open PID, ">data/online/pids/$room/$pid";
        print PID time;
        close PID;
    }
    else {
        sleep 1;
        goto OPENMSGFILE;
    }
    &sendmsgs;
    while (-f "data/online/users/$alias") {
        local $SIG{'USR1'} = \&sendmsgs;
        goto OPENMSGFILE if $change_room == 1;
        unlink "data/online/rooms/$loginplace/$alias"
         if (-f "data/online/rooms/$loginplace/$alias");
        sleep 15;
        print "\n";
    }
    sub sendmsgs {
        for ($curpos = tell MSGFILE; defined($_ = <MSGFILE>); $curpos = tell MSGFILE) {
            my($address, $stamp, $command, $action, $message, $from) = split(/<;/, $_, 7);
            if ($address =~ /$\!/ or $address eq $alias) {
                if ($stamp > $startime) {
                    if ($command eq 'col') {
                        ($col, $secol) = split(/;/, $action, 3);
                    }
                    elsif ($command eq 'cr') {
                        unlink "data/online/pids/$room/$pid";
                        $room = $action;
                        close MSGFILE;
                        $change_room = 1;
                    }
                    elsif ($command eq 'ig') {
                        my @temp;
                        my $add2list = 1;
                        foreach $_ (@ignore) {
                            if ($action ne $_) {
                                push @temp, $_;
                            }
                            else {
                                $add2list = 0;
                            }
                        }
                        if ($add2list == 1) {
                            push @ignore, $action;
                            print "<font color='#ffffff'><font color='#ffa500'>$action</font> wird nun ignoriert!</font><br>\n";
                            &post_prv_extra($action, "<font color='#ff0000'>Du wirst nun von $alias ignoriert!</font>");
                        }
                        else {
                            @ignore = @temp;
                            print "<font color='#ffffff'><font color='#ffa500'>$action</font> wird nun nicht mehr ignoriert!</font><br>\n";
                            &post_prv_extra($action, "<font color='#ff0000'>Du wirst nun von $alias nicht mehr ignoriert!</font>");
                        }
                    }
                    elsif ($command eq 'igl') {
                        my $ignored;
                        foreach $_ (@ignore) {
                            $ignored .= "$_ ";
                        }
                        if ($#ignore == -1) {
                            print "<font color='#ffffff'>Momentan wird keine Person von dir ignoriert!</font><br>\n";
                        }
                        else {
                            print "<font color='#ffffff'>Folgende Personen werden von dir ignoriert: <font color='#ffa500'>$ignored</font>!</font><br>\n";
                        }
                    }
                    elsif ($command eq 'quit') {
                        print "<script language='JavaScript'>\n";
                        print "open('chat?request=online&room=$loginplace&alias=$alias&tmpid=$tmpid','online');\n";
                        print "open('chat?request=htdocs','input');\n";
                        print "open('chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder','main');\n";
                        print "</script>\n";
                        last;
                    }
                    elsif ($change_room != 1) {
                        my $ignored = 0;
                        foreach $_ (@ignore) {
                            if ($_ eq $from) {
                                $ignored = 1;
                                last;
                            }
                        }
                        if ($ignored != 1) {
                            print "$message\n";
                        }
                        else {
                            $address =~ s/!//;
                            &post_prv_extra($from, "<font color='#ff0000'>Du wirst von $alias ignoriert!</font>") if $address eq $alias;
                        }
                    }
                    push @flood, $address . $command . $action . $message . $from;
                    if ($flood[0] eq $flood[1] and $flood[0] eq $flood[2] and $flood[0] eq $flood[3]) {
                        print "<i><font color=red>$from flutet den Chat !</font></i><br>\n";
                        if ($from eq $alias and $flooding++ and $flooding >= 4) {
                            &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='ffa500'><b>Das System</b></font> katapultiert <font color='".&get_ident($alias,3)."'>$alias</font>> wegen Fluten des Chats nach draussen ... </font></i>", $alias);
                            print "<script language='JavaScript'>open('chat?request=start&tmp=autokick','_top');</script>\n";
                            &rm_alias($alias, $room);
                        }
                    }
                    else {
                        $flooding = 0;
                    }
                    shift @flood if $#flood > 2;
                }
            }
        }
        seek MSGFILE, $curpos, 0;
    }
    unlink "data/online/pids/$room/$pid";
}
sub post_prv_extra {
    my($alias, $message) = @_;
    my $helpid = fork;
    if ($helpid == 0) {
        &post_prv($alias, $message);
        exit 0;
    }
}
1;
