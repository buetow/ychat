sub loggedin {
    &start_html($loginplace, 'blank');
    print qq[<div>\n <font color='$col'>$alias</font>, sch\366n da\337 Du da bist :-), <a href='chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder' style="text-decoration: underline; color: red">hier</a> kannst du die Seite aktualisieren.\n</div>\n<hr>\n<div>\n<a href="chat?request=stream&alias=$alias&room=$standardroom&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder" style="text-decoration: underline; color: red; font-size:20px">Hier</a>\nkannst du den Standardraum $standardroom betreten. <br><br>\n];
    opendir ROOMDIRS, 'data/online/rooms';
    local(@roomdirs) = readdir ROOMDIRS;
    close ROOMDIRS;
    foreach $_ (@roomdirs) {
        if (-e "data/online/rooms/$_" and $_ ne '..' and $_ ne '.') {
            my $room = $_;

            open RSTAT, "<data/online/rstat/$room";
            my(@rstat) = <RSTAT>;
            close RSTAT;

            my $door = '[<font color=red>Abgesperrt!</font>]' if $rstat[0] eq "1\n";

            opendir ROOMDIR, "data/online/rooms/$room";
            my(@roomdir) = readdir ROOMDIR;
            close ROOMDIR;

            my $nr = $#roomdir - 1;
            if ($door or $_ eq $loginplace) {
                print $q->b($_), " $door ($nr) ";
            }
            else {
                print $q->b($q->a({-'href', "chat?request=stream&alias=$alias&room=$_&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder"}, "$_")), " ($nr)";
            }
            print " Raumthema: $rstat[1]" if $rstat[1] ne "\n";
            print $q->br;
            foreach $_ (@roomdir) {
                if ($_ ne '..' and $_ ne '.') {
		    my $col = &get_ident($_,3);
                    print "<a href='chat?request=divcom&var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid' target='$_' style='color: $col'>$_</a> ";
                }
            }
            print $q->br, $q->br;
        }
    }
    &gb_html($_[0],'loggedin');
}
1;
