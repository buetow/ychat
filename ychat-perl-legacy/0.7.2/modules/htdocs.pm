sub htdocs {
    if ($var eq 'help_html') {
        &printfile('templates/help.sno', 'Alle Chatbefehle');
    }
    elsif ($var eq 'logo_html') {
        &start_html('Logo');
        &logo_html;
    }
    elsif ($var eq 'menu_html') {
        &menu_html;
    }
    elsif ($var eq 'logs_html') {
        &logs_html;
    }
    elsif ($var eq 'view_logs_html') {
        &secure_checkid($alias, $tmpid);
        &printfile("data/logs/$tmp", 'Logdatei', 'blank') if 5 <= &hierachie($alias);
    }
    elsif ($var eq 'superuser_html') {
        &secure_checkid($alias, $tmpid);
        &superuser_html;
    }
    elsif ($var eq 'chatiquette_html') {
        &printfile('templates/chatiq.sno', 'Chatiquette');
        print $q->end_html;
    }
    else {
        print $q->start_html(-'bgcolor', '000000'), $q->end_html;
    }
}
sub menu_html {
    &start_html("Men\374");
    print "<div>\n Von hier aus kannst du alle Untermen\374punkte des Systems erreichen:\n</div>\n<hr>\n";
    &menu();
    print "</body>\n</html>";
}
sub logo_html {
    print qq[ <layer top=-3 left=0><div style="position:absolute; top:5; left:10">\n <OBJECT classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"\n  codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=5,0,0,0"\n  WIDTH=48 HEIGHT=48>\n  <PARAM NAME=movie VALUE="$gfxpath/ySymbol.swf">\n  <PARAM NAME=loop VALUE=false>\n  <PARAM NAME=menu VALUE=false>\n  <PARAM NAME=quality VALUE=high>\n  <PARAM NAME=scale VALUE=noborder>\n  <PARAM NAME=wmode VALUE=opaque>\n  <PARAM NAME=bgcolor VALUE=$bgcolor>\n  <EMBED src="$gfxpath/ySymbol.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=$bgcolor  WIDTH=48 HEIGHT=48 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>\n </OBJECT>\n </div></layer>\n];
    print $q->end_html;
}
sub logs_html {
    &start_html('Logdateien');
    &menu;
    print "<div>\n Hier befinden sich alle Logdateien\n</div>\n<hr>\n";
    if (5 <= &hierachie($alias)) {
        print $q->div(qq[Folgende Logdateien des Raumes "$standardroom" k\366nnen nur von Oberw\344chtern oder h\366her gelesen werden (Sortiert nach A-Z, a-z):]), $q->br;
        opendir LOGS, 'data/logs';
        my(@logs) = readdir LOGS;
        closedir LOGS;
        my(@sortedir) = sort(@logs);
        foreach $_ (@sortedir) {
            if (-f "data/logs/$_") {
                print $q->a({-'href', "chat?request=htdocs&alias=$alias&var=view_logs_html&tmp=$_&tmpid=$tmpid", -'target', '_new'}, "$_"), ' ';
            }
        }
    }
    else {
        print $q->div("Logdateien k\366nnen nur von Oberw\344chtern (OW) gelesen werden. Du bist jedoch ein W\344chter (W\304) oder ein", qq["Normaler Chatter". Oberw\344chter k\366nnen alles nachlesen, was \366ffentlich im Raum "Cyberbar" gechattet wurde.]), $q->hr;
    }
    print $q->end_html;
}
sub superuser_html {
    &start_html ('Superuser');
    &menu;
    print "<div>Hallo $alias. Du bist als ";
    my $hierachie = &hierachie($alias);
    if (7 <= $hierachie) {
        print $q->b('Administrator'), ' (AD) eingetragen.</div><br>', $q->br;
    }
    elsif (5 <= $hierachie) {
        print $q->b("Oberw\344chter"), ' (OW) eingetragen.</div><br>', $q->br;
    }
    elsif (3 <= $hierachie) {
        print $q->b("W\344chter"), " (W\304) eingetragen.</div><br>", $q->br;
    }
    else {
        print $q->b('normaler Chatter'), ' (NC) eingetragen.</div><br>', $q->br;
    }
    open FILE, 'data/hierachie';
    my(@hierachie) = <FILE>;
    close FILE;
    my @ows, @was, @ads;
    foreach $_ (@hierachie) {
        my($name, $level) = split(/<;/, $_, 3);
        chomp $level;
        if ($level == 3) {
            push @was, $name;
        }
        elsif ($level == 5) {
            push @ows, $name;
        }
        elsif ($level == 7) {
            push @ads, $name;
        }
        if (3 <= $hierachie) {
            if ($level == 4) {
                push @was, "$name!";
            }
            elsif ($level == 6) {
                push @ows, "$name!";
            }
            elsif ($level == 8) {
                push @ads, "$name!";
            }
        }
    }
    my $nads = $#ads + 1;
    print '<div>', $q->b('AD'), "s ($nads St\374ck)";
    foreach $_ (@ads) {
        print $q->font({-'color', 'ffa500'}, ' ' . $_);
    }
    my $nrow = $#ows + 1;
    print '</div><br><br><div>', $q->b('OW'), "s ($nrow St\374ck)";
    foreach $_ (@ows) {
        print $q->font({-'color', 'ffa500'}, ' ' . $_);
    }
    my $nrwa = $#was + 1;
    print '</div><br>', $q->br, '<div>', $q->b("W\304"), "s ($nrwa St\374ck)";
    foreach $_ (@was) {
        print $q->font({-'color', 'FFA500'}, ' ' . $_);
    }
    print qq[<hr>\n Die jeweiligen F\344higkeiten der Oberw\344chter (OW) und der W\344chter (W\304) sind unter\n "<a href="chat?request=htdocs&var=help_html">Chat-Befehle</a>"\n dokumentiert. Versteckte Superuser k\366nnen nur von anderen Superusern auf\ndieser Liste eingesehen werden!\n</div>\n</body>\n</html>\n];
}
sub menu {
    print "<div>\n  <a href='chat?request=divcom&var=edit_profile_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Profil \344ndern</a>\n  <a href='chat?request=divcom&var=edit_options_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Optionen</a>\n  <a href='chat?request=divcom&var=list_alias_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Benutzer</a>\n  <a href='chat?request=htdocs&var=help_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Befehle</a>\n  <a href='chat?request=divcom&var=gb_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Board</a>\n  <a href='chat?request=htdocs&var=superuser_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Superuser</a>\n  <a href='chat?request=htdocs&var=logs_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Logs</a>\n  <a href='chat?request=statistic&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Statistik</a>\n  <a href='chat?request=htdocs&var=chatiquette_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Chatiquette</a>\n</div>\n<hr>\n";
}
1;
