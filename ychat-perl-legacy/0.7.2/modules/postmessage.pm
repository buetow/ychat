sub postmessage {
    if (not -e "data/online/rooms/$room/$alias" and $tmp ne 'autoaway' and $tmp ne 'autologout') {
        &post_prv($alias, "<font color=ffffff>Du befindest Dich nicht im Raum <font color=ffa500>$room</font> und kannst dort auch keine Nachrichten posten!</font>");
        &reload_html;
    }
    elsif (length $msg > 300) {
        &post_prv($alias, "<font color=ffffff>Deine Nachricht enth\344lt zu viele Zeichen. Maximal sind 300 Zeichen erlaubt.</font>");
        &reload_html;
    }
    if ($tmp eq 'away') {
        &rm_away($alias, $room);
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> hat sich zur\374ck gemeldet ... </font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
        &private_javascript($alias, $room, q[parent.input.document.inf.tmp.value = 'notaway';]);
    }
    $msg =~ s/</&lt;/g;
    if (substr($msg, 0, 1) ne '/') {
        unless ($tmp eq 'autoaway') {
            foreach $_ ($col, $secol) {
                &prove_color($alias, $_);
            }
        }
        open MSGFILE, ">>data/msgs/$room";
        if (not $msg =~ s[/me/][<i>$alias</i>]g || $msg =~ s[/m/][<i><font color=$col>$alias</font></i>]g) {
            print MSGFILE '!<;' . time . "<;!<;!<;<b><font color='$col'>$alias: <font color='$secol'>$msg<br></font></font></b><;$alias<;\n";
        }
        else {
            $msg =~ s[/me/][<i>$alias</i>]g;
            $msg =~ s[/m/][<i><font color=$col>$alias</font></i>]g;
            print MSGFILE '!<;' . time . "<;!<;!<;<b><font color=$secol>$msg</font></font></b><br><;$alias<;\n";
        }
        close MSGFILE;
        opendir PID, "data/online/pids/$room";
        my(@pids) = readdir PID;
        closedir PID;
        foreach $_ (@pids) {
            kill 'USR1', $_ if -f "data/online/pids/$room/$_";
        }
        &log("<b><font color=$col>$alias: <font color=$secol>$msg</font></font></b>") if $room eq $standardroom;
        &reload_html;
    }
    else {
        &command($msg, $tmp);
    }
}
sub post {
    my($room, $msg2post, $alias, $secroom) = @_;
    my(@rooms) = $room;
    @rooms = ($room, $secroom) if $room ne $secroom;
    foreach my $theroom (@rooms) {
        open MSGFILE, ">>data/msgs/$theroom";
        print MSGFILE '!<;' . time . "<;!<;!<;$msg2post<br><;$alias<;\n";
        close MSGFILE;
        opendir PID, "data/online/pids/$theroom";
        my(@pids) = readdir PID;
        closedir PID;
        foreach $_ (@pids) {
            kill 'USR1', $_ if -f "data/online/pids/$theroom/$_";
        }
    }
    &log($msg2post) if $room eq $standardroom;
}
sub post_prv {
    my($alias2post, $msg2post) = @_;
    my $roomofalias = &get_room($alias2post);
    return 'false' if not $roomofalias;
    open MSGFILE, ">>data/msgs/$roomofalias";
    print MSGFILE "$alias2post<;" . time . "<;!<;!<;$msg2post<br><;$alias<;\n";
    close MSGFILE;
    opendir PID, "data/online/pids/$roomofalias";
    my(@pids) = readdir PID;
    closedir PID;
    foreach $_ (@pids) {
        kill 'USR1', $_ if -f "data/online/pids/$roomofalias/$_";
    }
    return 'true';
}
sub post_strmsg {
    my($alias, $room, $command, $action) = @_;
    open MSGFILE, ">>data/msgs/$room";
    print MSGFILE "$alias<;" . time . "<;$command<;$action<;!<;!<;\n";
    close MSGFILE;
    opendir PID, "data/online/pids/$room";
    my(@pids) = readdir PID;
    closedir PID;
    foreach $_ (@pids) {
        kill 'USR1', $_ if -f "data/online/pids/$room/$_";
    }
}

1;
