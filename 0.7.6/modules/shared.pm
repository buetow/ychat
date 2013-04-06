sub reload_html {
    print "<html><body bgcolor='$chatbg'></body></html>";
    $client->shutdown(2);
    exit 0;
}
sub printfile {
    my($file2print, $pagetitle, $bodyclass) = @_;
    &start_html($pagetitle, $bodyclass) if $pagetitle ne '';
    &menu unless $bodyclass eq 'blank';
    open FILE2PRINT, "<$file2print";
    print "$_\n" while defined($_ = <FILE2PRINT>);
    close FILE2PRINT;
}
sub start_html {
    print "<html><head><title>$title -  $_[0]</title>$_[2]";
    &style;
    print '</head>';
    if ($_[1] eq 'start') {
        print '<body onload="document.login.alias.focus();">';
    }
    elsif ($_[1] ne '') {
        print "<body class=$_[1]>";
    }
    else {
        print '<body>';
    }
}
sub log {
    my $msg2log = $_[0];
    my($secs, $mins, $hours, $days, $months, $years) = &timearray;
    my $js;
    ($msg2log, $js) = split(/<script/, $msg2log, 3);
    open LOG, ">>data/logs/$days.$months.$years";
    print LOG "<br><font color=ffffef><i>($hours:$mins:$secs)</i></font> $msg2log\n";
    close LOG;
}
sub timestring {
    my($mins, $hours) = (localtime)[1, 2];
    $hours = '0' . $hours if $hours < 10;
    $mins = '0' . $mins if $mins < 10;
    return "$hours:$mins";
}
sub timearray {
    my($secs, $mins, $hours, $days, $months, $years) = (localtime)[(0, 1, 2, 3, 4, 5)];
    $months += 1;
    $hours = '0' . $hours if $hours < 10;
    $mins = '0' . $mins if $mins < 10;
    $secs = '0' . $secs if $secs < 10;
    $days = '0' . $days if $days < 10;
    $months = '0' . $months if $months < 10;
    $years = $years - 100;
    if ($years < 10) {
        $years = '200' . $years;
    }
    else {
        $years = '20' . $years;
    }
    return $secs, $mins, $hours, $days, $months, $years;
}
sub error {
    my $error_msg = $_[0];
    &start_html("Error: ($error_msg)");
    print $q->div("Error: ($error_msg)"), $q->end_html;
    open ERROR, '>>data/error';
    print ERROR "Alias: $alias TempID: $tmpid File. $0 PID: $$ Time: " . time . " Message: $error_msg \n";
    close ERROR;
    $client->shutdown(2) if $client;
    exit 0;
}
sub check_online {
    open PROVE, '>data/online/prove';
    print PROVE time;
    close PROVE;
    opendir DIR, 'data/online/rooms';
    my(@roomdir) = readdir DIR;
    closedir DIR;
    foreach my $theroom (@roomdir) {
        opendir DIR, "data/online/rooms/$theroom";
        my(@roomusers) = readdir DIR;
        closedir DIR;
        my $empty = 1;
        foreach $user (@roomusers) {
            if (-f "data/online/rooms/$theroom/$user") {
                $empty = 0;
                open USER, "<data/online/rooms/$theroom/$user";
                my $userstamp = <USER>;
                close USER;
                if ($userstamp < time - 40) {
                    unlink "data/online/$theroom/$user";
                    open USER, "<data/online/users/$user";
                    my $userstamp = <USER>;
                    close USER;
                    if ($userstamp < time - 40) {
                        &post($theroom, '<i><font color=ffffff>' . &timestring . " <font color='".&get_ident($user,3)."'>$user</font> hat den Chat verlassen ... </font></i>");
                        if ($user ne $alias) {
                            &rm_alias($user, $theroom);
                        }
                        else {
                            unlink "data/online/rooms/$theroom/$user";
                        }
                    }
                }
            }
        }
        opendir PIDS, "data/online/pids/$theroom";
        my(@pids) = readdir PIDS;
        closedir PIDS;
        if ($empty == 1) {
            rmdir "data/online/rooms/$theroom";
            unlink "data/online/rstat/$theroom";
            unlink "data/online/rstat/$theroom.away";
            unlink "data/msgs/$theroom";
            foreach $_ (@pids) {
                unlink "data/online/pids/$theroom/$_" if -f "data/online/pids/$theroom/$_";
            }
            rmdir "data/online/pids/$theroom";
        }
    }
}
sub rm_if_loggedin {
    my $alias = shift;
    my $room = &get_room($alias);
    while ($room ne undef) {
    	&rm_alias($alias, $room);
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$user</font> hat den Chat verlassen ... </font></i>");
        $room = &get_room($alias);    
    }
}
sub secure_checkid {
    my($alias2check, $tmpid) = @_;
    &error("Falsche TempID! $tmpid") unless -f "data/online/tmpid/$alias.$tmpid";
}
sub rm_alias {
    my($alias, $room) = @_;
    unlink "data/online/rooms/$room/$alias";
    unlink "data/online/users/$alias";
    opendir TMPID, 'data/online/tmpid';
    my(@tmpid) = readdir TMPID;
    close TMPID;
    foreach $_ (@tmpid) {
        unlink "data/online/tmpid/$_" if /^$alias\..+$/;
    }
    unlink "data/online/ident/$alias";
    &rm_rstat($alias, $room);
}
sub rm_rstat {
    my($alias, $room) = @_;
    open RSTAT, "<data/online/rstat/$room";
    my(@rstat) = <RSTAT>;
    close RSTAT;
    my(@rstat2) = ($rstat[0], $rstat[1]);
    for ($i = 2; $i <= $#rstat; ++$i) {
        chomp $rstat[$i];
        push @rstat2, $rstat[$i] . "\n" unless $rstat[$i] =~ /^$alias<;/;
    }
    open RSTAT, ">data/online/rstat/$room";
    print RSTAT @rstat2;
    close RSTAT;
}
sub rm_away {
    my($alias, $room) = @_;
    open AWAY, "<data/online/rstat/$room.away";
    my(@away) = <AWAY>;
    close AWAY;
    my @away2;
    foreach $_ (@away) {
        my(@split) = split(/<;/, $_, 0);
        push @away2, $_ if $alias ne $split[0];
    }
    open AWAY, ">data/online/rstat/$room.away";
    print AWAY @away2;
    close AWAY;
}
sub rm_entry {
    my($entry, $file) = @_;
    open ENTRY, "<data/$file";
    my(@entry) = <ENTRY>;
    close ENTRY;
    my @new;
    foreach $_ (@entry) {
        push @new, $_ if $entry . "\n" ne $_;
    }
    if (@entry != @new) {
        open ENTRY, ">data/$file";
        print ENTRY @new;
        close ENTRY;
        return 1;
    }
    else {
        return;
    }
}
sub add_entry {
    my($entry, $file) = @_;
    open ENTRY, "<data/$file";
    my(@entry) = <ENTRY>;
    close ENTRY;
    foreach $_ (@entry) {
        chomp $_;
        return if $entry eq $_;
    }
    open ENTRY, ">>data/$file";
    print ENTRY $entry . "\n";
    close ENTRY;
    return 1;
}
sub prove_color {
    my($alias, $color) = @_;
    my $length = length $color;
    if ($length > 6 or $length < 6) {
        &post_prv($alias, "<font color=ffffff>Ung\374ltige Hexadezimalfarbe.</font>");
        &reload_html;
    }
}
sub kill_alias {
    my($alias2kill, $alias) = @_;
    my($secs, $mins, $hours, $days, $months, $years) = &timearray;
    unlink "data/user/options/$alias2kill";
    unlink "data/user/profiles/$alias2kill";
    unlink "data/user/gbooks/$alias2kill";
    unlink "data/user/counts/$alias2kill";
    unlink "data/user/counts/$alias2kill.online";
    unlink "data/user/lastlogin/$alias2kill";
    unlink "data/user/memos/$alias2kill";
    &rm_entry($alias2kill, 'ban');
    open DEL, '>>data/kill';
    print DEL "($days.$months.$years $hours:$mins:$secs) $alias2kill wurde von $alias gekillt\n";
    close DEL;
}
sub random_color {
    my(@digit) = ('F', 'C', 'A', 'B', (5, 6, 7, 8, 9));
    my $dig1 = rand @digit;
    my $dig2 = rand @digit;
    my $dig3 = rand @digit;
    my $dig4 = rand @digit;
    my $dig5 = rand @digit;
    my $dig6 = rand @digit;
    return $digit[$dig1] . $digit[$dig2] . $digit[$dig3] . $digit[$dig4] . $digit[$dig5] . $digit[$dig6];
}
sub url_decode {
    my $t = shift @_;
    $t =~ s[/][];
    $t =~ tr/+/ /;
    while ($t =~ s/%([a-f0-9][a-f0-9])/chr hex $1;/ie) {
        ();
    }
    return $t;
}
sub url_encode {
    my $t = shift @_;
    while ($t =~ s/([^\w()'*~!.-])/sprintf '%%%02x', $1;/eg) {
        ();
    }
    return $t;
}
sub calculate_hpm {
    my($hits, $starttime) = @_;
    return sprintf('%.2f', $hits / ((time - $starttime) / 60));
}
sub calculate_uptime_hours {
    my $starttime = shift @_;
    return sprintf('%.2f', (time - $starttime) / 3600);
}
sub private_javascript {
    my($alias, $room, $javascript) = @_;
    open MSGFILE, ">>data/msgs/$room";
    print MSGFILE "$alias<;" . time . qq[<;!<;!<;<script language="JavaScript">$javascript</script><;\n];
    close MSGFILE;
    opendir PID, "data/online/pids/$room";
    my(@pids) = readdir PID;
    closedir PID;
    foreach $_ (@pids) {
        kill 'USR1', $_ if -f "data/online/pids/$room/$_";
    }
}
sub get_theme {
    my $room = shift @_;
    open RSTAT, "data/online/rstat/$room";
    my(@rstat) = <RSTAT>;
    close RSTAT;
    chomp $rstat[1];
    return '<b>Raumthema:</b>' . $rstat[1] if $rstat[1] ne undef;
}
sub get_room {
    my $alias = shift @_;
    opendir DIR, 'data/online/rooms';
    my(@dir) = readdir DIR;
    closedir DIR;
    foreach my $room (@dir) {
        opendir DIR, "data/online/rooms/$room";
        my(@chatter) = readdir DIR;
        closedir DIR;
        foreach my $chatter (@chatter) {
            return $room if $chatter eq $alias;
        }
    }
    return undef;
}
sub get_ident {
   my ($alias,$line) = @_;
   open IDENT, "data/online/ident/$alias";
   my (@ident) = <IDENT>;
   close IDENT;
   chomp $ident[$line];
   return $ident[$line];
}
sub get_online_users {
   my @dir = &diropen('data/online/rooms');
   my @users;

   foreach my $room (@dir) {
   	next if $room =~ /\./;
	my @room = &diropen("data/online/rooms/$room");
        
	foreach (@room) {
		push @users, $_ unless /\./;
	}
   }
   return @users;
}
sub diropen {
   my $dir = shift;

   opendir DIR, $dir;
   my @dir = readdir(DIR);
   closedir(DIR);

   return @dir;
}
sub debug {
    open DEBUG, '>data/debug';
    while (@_) {
        chomp $_;
        print DEBUG "$_\n";
    }
    close DEBUG;
}
1;
