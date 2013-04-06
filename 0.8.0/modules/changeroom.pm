sub change_room {
    my($method, $newroom, $oldroom, $alias2) = @_;
    if ($newroom eq $oldroom) {
        &post_prv($alias, "<font color='ffffff'>Du befindest dich bereits im Raum <font color='ffa500'>$newroom</font>!</font>");
        &reload_html;
    }
    elsif ($newroom eq $loginplace) {
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> verl&auml;sst den Chat und geht in die <b>$loginplace</b> ... </font></i>", $alias);
        &post_strmsg($alias, $room, 'quit');
        unlink "data/online/rooms/$room/$alias";
        &rm_rstat($alias, $room);
        &check_online;
        &reload_html;
    }
    open RSTAT, "<data/online/rstat/$newroom";
    my(@rstat) = <RSTAT>;
    close RSTAT;
    if ($rstat[0] eq "1\n") {
        if (3 <= &hierachie($alias)) {
            &post_prv($alias, "<font color='ffffff'>Der Raum <font color='ffa500'>$newroom</font> ist leider abgesperrt!</font>");
            &reload_html;
        }
    }
    &private_javascript($alias, $room, "parent.input.document.inf.room.value = '$newroom';open('chat?request=online&room=$newroom&alias=$alias&tmpid=$tmpid','online');");
    open MSGFILE, ">>data/msgs/$newroom";
    print MSGFILE "$alias<;" . time . "<;!<;!<;<;\n";
    close MSGFILE;
    open MSGFILE, ">>data/msgs/$room";
    if ($method eq 'j') {
        print MSGFILE "$alias<;" . time . q[<;!<;!<;<i><font color='ffffff'>] . &timestring . " Du wechselst aus dem Raum <b>$oldroom</b> herein nach <b>$newroom</b> ...</font></i><br><;\n";
    }
    elsif ($method eq 'ju') {
        print MSGFILE "$alias<;" . time . q[<;!<;!<;<i><font color='ffffff'>] . &timestring . " Du folgst <font color='".&get_ident($alias2,3)."'>$alias2</font> aus dem Raum <b>$oldroom</b> herein nach <b>$newroom</b> ...</font></i><br><;\n";
    }
    elsif ($method eq 'sep') {
        print MSGFILE "$alias<;" . time . q[<;!<;!<;<i><font color='ffffff'>] . &timestring . " Du wechselst aus dem Raum <b>$oldroom</b> herein nach <b>$newroom</b> und sperrst ab...</font></i><br><;\n";
    }
    close MSGFILE;
    &post_strmsg($alias, $room, 'cr', $newroom);
    $room = $oldroom;
    my $tmpstamp = time;
    if ($method eq 'j') {
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> wechselt in den Raum <b>$newroom</b> ...</font></i>", $alias);
        $room = $newroom;
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> kommt aus dem Raum <b>$oldroom</b> herein nach <b>$newroom</b> ...</font></i>", $alias);
    }
    elsif ($method eq 'ju') {
        my $col2 = &get_ident($alias2,3);
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> folgt <font color='$col2'>$alias2</font> in den Raum <b>$newroom</b> ...</font></i>", $alias);
        $room = $newroom;
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> kommt aus dem Raum <b>$oldroom</b> und folgt <font color='$col2'>$alias2</font> herein nach <b>$newroom</b> ...</font></i>", $alias);
    }
    elsif ($method eq 'sep') {
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> wechselt in den Raum <b>$newroom</b> und sperrt ab...</font></i>", $alias);
        $room = $newroom;
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias</font> kommt aus dem Raum <b>$oldroom</b> herein nach <b>$newroom</b> und sperrt ab...</font></i>", $alias);
    }
    elsif ($method eq 'beam') {
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias2</font> beamt <b>$alias</b> in den Raum  <b>$newroom</b>...</font></i>", $alias2);
        $room = $newroom;
        &post($room, q[<i><font color='ffffff'>] . &timestring . " <font color='$col'>$alias2</font> beamt <b>$alias</b> aus dem Raum <b>$oldroom</b> herein nach <b>$room</b>...</font></i>", $alias2);
    }
    unlink "data/online/rooms/$oldroom/$alias";
    &rm_rstat($alias, $oldroom);
    &check_online;
}
1;
