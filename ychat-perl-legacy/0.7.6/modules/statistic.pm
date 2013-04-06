sub statistic {
    &start_html('Statistiken');
    &menu;
    print $q->div('yChat Statistiken'), $q->hr, $q->div($q->a({-'href', "chat?request=statistic&var=ranking&alias=$alias&tmpid=$tmpid"}, 'Rankings'), ' | ', $q->a({-'href', "chat?request=statistic&var=del&alias=$alias&tmpid=$tmpid"}, 'Loginstatistiken'), ' | ', $q->a({-'href', "chat?request=statistic&var=statistic&alias=$alias&tmpid=$tmpid"}, 'DB-Statistik'), ' | ', $q->a({-'href', "chat?request=statistic&var=var&alias=$alias&tmpid=$tmpid"}, 'Umgebungsvariablen')), $q->hr;
    if ($var eq 'ranking') {
        opendir COUNTS, 'data/user/counts';
        my(@counts) = readdir COUNTS;
        closedir COUNTS;
        print '<div>';
        my $nr_start = $start;
        my $nr_end = $end;
        my %online;
        my $rank;
        my $nr;
        foreach $_ (@counts) {
            if (-f "data/user/counts/$_") {
                if (s/.online//g) {
                    open TOTAL, "<data/user/counts/$_.online";
                    my $total = <TOTAL>;
                    close TOTAL;
                    $online{sprintf '%.2f', $total / 3600} = $_;
                }
            }
        }
        print '<div>', $q->b('Rankings:'), $q->br, '<table><tr><td><div><b>#:</b></div></td><td><b><div>St.:</div></b></td><td><b><div>Name:</div></b></td></tr>';
        if (not $nr_start) {
            $nr_start = 1;
        }
        if (not $nr_end) {
            $nr_end = 20;
        }
        foreach $wert (reverse (sort { $a <=> $b } keys %online)) {
            ++$nr;
            if ($online{$wert} eq $alias) {
                $rank = $nr;
                last if $nr > $nr_end;
            }
            next if ($nr < $nr_start or $nr > $nr_end);
            print q[<tr><td><font color='ffffff' size='1' face='arial'>] . $nr . q[</font></td><td><font color='ffffff' size='1' face='arial'> ] . $wert . "h </font></td><td> <a href='chat?request=divcom&var=alias_html&alias=$alias&tmp=$online{$wert}&tmpid=$tmpid'><font size='1' face='arial'>$online{$wert}</font></a></td></tr>\n";
        }
        print '</table>', $q->hr;
        $nr_start -= 20;
        $nr_end -= 20;
        print "Rankings von <a href='chat?request=statistic&var=ranking&start=$nr_start&end=$nr_end&alias=$alias&tmpid=$tmpid'>$nr_start bis $nr_end</a>  anzeigen ...<br>" if $nr_start > 0;
        $nr_start += 40;
        $nr_end += 40;
        print "Rankings von <a href='chat?request=statistic&var=ranking&start=$nr_start&end=$nr_end&alias=$alias&tmpid=$tmpid'>$nr_start bis $nr_end</a>  anzeigen ...", $q->br, $q->br, $q->b("$alias, Du befindest Dich auf Rank $rank</div>");
    }
    elsif ($var eq 'del') {
        print '<div>', $q->b('Alte Benutzer l&ouml;schen:'), $q->br, "Benutzer, die sich 1440 Stunden (60 Tage) nicht mehr in das System eingeloggt haben, werden gel\366scht.", $q->br, $q->br;
        opendir USER, 'data/user/options';
        my(@user) = readdir USER;
        closedir USER;
        @user = sort(@user);
        my $i = 0;
        my $j;
        my $k = 0;
        my $l = 0;
        my $full = $tmp;
        foreach $_ (@user) {
            if (-f "data/user/lastlogin/$_") {
                open LOGIN, "<data/user/lastlogin/$_";
                my $login = <LOGIN>;
                close LOGIN;
                $login = sprintf('%.2f', (time - $login) / 3600);
                ++$j;
                if (999 < $login and $login < 1440) {
                    ++$k;
                    print $q->font({-'color', 'ffa500'}, $q->b("$j($k) $_ Letztes Login vor: " . $login . 'h')), $q->br, "\n" if $full ne undef;
                }
                elsif ($login < 48) {
                    ++$l;
                    print $q->font({-'color', 'ffffef'}, $q->b("$j($l) $_ Letztes Login vor: " . $login . 'h')), $q->br, "\n" if $full ne undef;
                }
                elsif ($login < 1440) {
                    print "$j $_ Letztes Login vor: " . $login . 'h', $q->br, "\n" if $full ne undef;
                }
                else {
                    ++$k;
                    &kill_it($_, $i++, $login);
                }
            }
            elsif (-f "data/user/options/$_") {
                &kill_it($_, $i++);
            }
        }
        print $q->hr if $full ne undef;
        print '</div>', $q->div($q->b("$j Benutzer befinden sich in der Datenbank!", $q->br, $q->font({-'color', 'ffffef'}, "$l Benutzer haben sich innerhalb der letzten 48 Stunden  eingeloggt!"), $q->br, $q->font({-'color', 'ffa500'}, "$k Benutzer haben sich &uuml;ber 1000 Stunden nicht mehr eingeloggt!"), $q->br, $q->font({-'color', 'ff0000'}, "$i Benutzer wurden gerade eben gel\366scht!")), $q->br, $q->br, 'PS: Jeder registrierte Benutzername wird nur einmal gez&auml;hlt!');
        print $q->br, $q->div('(', $q->a({-'href', "chat?request=statistic&var=del&tmp=true&alias=$alias&tmpid=$tmpid"}, 'Vollst&auml;ndige Ansicht'), ')') if $full eq undef;
    }
    elsif ($var eq 'var') {
        print '<div>', $q->b('Server Umgebungsvariablen:'), $q->br, $q->br, 'Die Serverumgebungsvariablen k&ouml;nnen nur von Oberw&auml;chtern oder h&ouml;her eingesehen werden!', $q->br, $q->br;
        if (7 <= &hierachie($alias)) {
            while (($key, $value) = each %ENV) {
                print "$key = $value\n", $q->br;
            }
        }
        print '</div>';
    }
    elsif ($var eq 'statistic') {
        print $q->div($q->b('yChat-DB Statistik:')), $q->br;
        $files = 0;
        $size = 0;
        $directorys = 1;
        &zaehle('data');
        $size = sprintf('%.2f', $size / 1048576);
        print $q->div("Dateien insgesamt: $files", $q->br, "MB insgesamt: $size", $q->br, "Verzeichnisse insgesamt: $directorys");
    }
    print $q->end_html;
}
sub zaehle {
    local($dir, $data) = @_;
    $dir = "$dir/$data" if $data ne undef;
    opendir DATA, $dir;
    local(@data) = readdir DATA;
    closedir DATA;
    $files2 = 0;
    foreach $_ (@data) {
        if (-f "$dir/$_") {
            ++$files;
            ++$files2;
            $size += -s "$dir/$_";
        }
        elsif (-d "$dir/$_" and $_ ne '.' and $_ ne '..') {
            ++$directorys;
            &zaehle($dir, $_);
        }
    }
}
sub kill_it {
    my($rm, $nr, $login) = @_;
    $login = 'XXXXXX' if $login eq undef;
    print $q->font({-'color', 'ff0000'}, $q->b("$j($nr) $rm Letztes Login vor: " . $login . 'h (wurde gel&ouml;scht)')), $q->br, "\n";
    print q[<font size='1'>];
    print 'options ' if unlink "data/user/options/$rm";
    print 'profiles ' if unlink "data/user/profiles/$rm";
    print 'gbooks ' if unlink "data/user/gbooks/$rm";
    print 'counts ' if unlink "data/user/counts/$rm";
    print 'counts.online ' if unlink 'data/user/counts/' . $rm . '.online';
    print 'lastlogin ' if unlink "data/user/lastlogin/$rm";
    print 'memos' if unlink "data/user/memos/$rm";
    print '</font>', $q->hr, "\n";
    &zeit;
    open DEL, '>>data/kill';
    print DEL "($day.$month.$year $hours:$min:$sec) $rm wurde automatisch gekillt [$alias]\n";
    close DEL;
}
1;
