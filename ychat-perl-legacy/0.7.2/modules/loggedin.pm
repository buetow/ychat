sub loggedin {
    &start_html($loginplace, 'blank');
    print qq[<div>\n <font color='$col'>$alias</font>, sch\366n da\337 Du da bist :-), <a href='chat?request=loggedin&alias=$alias&col=$col&secol=$secol&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder'>hier</a> kannst du die Seite aktualisieren.\n</div>\n<hr>\n<div>\n<a href="chat?request=stream&alias=$alias&room=$standardroom&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder">Hier</a>\nkannst du den Standardraum $standardroom betreten. <br><br>\n];
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
                    print "<a href='chat?request=divcom&var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid' target='$_'>$_</a> ";
                }
            }
            print $q->br, $q->br;
        }
    }
    &gb_html($_[0],'loggedin');
#    print $q->br, $q->br, $q->b($q->font({-'color', 'ffa500'}, 'Aktuelles:')), $q->br, "Wie ihr seht, ist der neue yChat da. Allerdings ist es gut M\366glich, da\337 diese Version einige Bugs beinhaltet! ", q[Solltest du welche entdecken, dann schick doch bitte eine Mail an <a href='mailto:snooper@ychat.de'>snooper@ychat.de</a>.], '<br><br>Leider konnten noch nicht alle geplanten Features eingebaut werden. Dazu fehlte leider die Zeit.', " Denn mit steigendem Sourcecode leidet leider auch das schnelle Anpassungsverm\366gen neuer Ideen.<br><br>", "Aber egal, hauptsache der Chat l\344uft wieder :-) Mfg, Snooper ...</div>", $q->end_html;
}
1;
