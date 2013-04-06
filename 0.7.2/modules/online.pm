sub online {
    &secure_checkid($alias, $tmpid);
    my(@away) = &prove_away($alias, $room, 'return_list');
    for (;;) {
        for ($i = 0; $i <= $#away; $i += 2) {
            last if $away[$i] eq $alias or $room eq $loginplace;
        }
        open DATEI, "<data/online/users/$alias";
        my $oldstamp = <DATEI>;
        close DATEI;
        if ($oldstamp > time - 100) {
            open DATEI, "<data/user/counts/$alias.online";
            my $online = <DATEI>;
            close DATEI;
            my $total = time - $oldstamp + $online;
            open DATEI, ">data/user/counts/$alias.online";
            print DATEI $total;
            close DATEI;
        }
        last;
    }
    unless (-d "data/online/rooms/$room") {
        mkdir "data/online/rooms/$room", 511;
    }
    open DATEI, ">data/online/users/$alias";
    print DATEI time;
    close DATEI;
    open DATEI, ">data/online/rooms/$room/$alias";
    print DATEI time;
    close DATEI;
    open DATEI, '<data/online/prove';
    my $provetime = <DATEI>;
    close DATEI;
    &check_online if $provetime < time - 40;
    print qq[<html>\n <head>\n  <meta http-equiv="refresh" content="20" URL="chat?request=online&room=$room&alias=$alias&tmpid=$tmpid">\n  <style type="text/css">\n   a { font-family: arial; font-size:11px; color: #ffffef; }\n   a:hover { color: #ffffff; }\n  </style>\n  <style type="text/css" media="all">\n   a { text-decoration: none; }\n   a:hover { text-decoration:underline; }\n  </style>\n </head>\n <body bgcolor="$chatbg">\n];
    opendir ROOM, 'data/online/rooms/' . $room;
    my(@room) = sort(readdir ROOM);
    closedir ROOM;
    my $chatter = $#room - 1;
    print "<font size='1' color='ffffff' face='arial'>$room ($chatter):</font><table>\n";
    open FILE, "<data/online/rstat/$room";
    my(@rstat) = <FILE>;
    close FILE;
    shift @rstat;
    shift @rstat;
    open FILE, '<data/hierachie';
    my(@userlevels) = (<FILE>, @rstat);
    close FILE;
    open FILE, "<data/online/rstat/$room.away";
    my(@away) = <FILE>;
    close FILE;
    foreach my $nick (@room) {
        if (-f "data/online/rooms/$room/$nick") {
            my $inside = 0;
            foreach $_ (@away) {
                my($name, $awaymessage) = split(/<;/, $_, 3);
                if ($name eq $nick) {
                    chomp $awaymessage;
                    print "<tr><td><img title='$awaymessage' alt='$awaymessage' src='$gfxpath/away.gif'></td>";
                    $inside = 2;
                }
            }
            if ($inside == 0) {
                foreach $_ (@userlevels) {
                    my($name, $level) = split(/<;/, $_, 3);
                    if ($nick eq $name) {
                        chomp $level;
                        $inside = 1;
                        if ($level == 1) {
                            print "<tr><td><img title='Raumbesetzer' alt='Raumbesetzer' src='$gfxpath/rb.gif'></td>";
                            last;
                        }
                        elsif ($level == 2) {
                            print "<tr><td><img title='Die Unschuld vom Land' alt='Die Unschuld vom Land' src='$gfxpath/uvl.gif'></td>";
                            last;
                        }
                        elsif ($level == 3) {
                            print "<tr><td><img title='W&auml;chter' alt='W&auml;chter' src='$gfxpath/wa.gif'></td>";
                            last;
                        }
                        elsif ($level == 5) {
                            print "<tr><td><img title='oberw&auml;chter' alt='Oberw&auml;chter' src='$gfxpath/ow.gif'></td>";
                            last;
                        }
                        elsif ($level == 7) {
                            print "<tr><td><img title='Administrator' alt='Administrator' src='$gfxpath/adm.gif'></td>";
                            last;
                        }
                        else {
                            print '<tr><td></td>';
                            last;
                        }
                    }
                }
            }
            if ($inside == 0) {
                print '<tr><td></td>';
            }
            print "<td><a href='chat?request=divcom&var=alias_html&alias=$alias&tmp=$nick&tmpid=$tmpid' target='$nick'>$nick</a></td></tr>\n";
        }
    }
    print '</table></body></html>';
}
1;
