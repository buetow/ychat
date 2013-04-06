sub hierachie {
    my($alias, $room) = @_;
    open FILE, "<data/online/rstat/$room";
    flock FILE, 2;
    my(@rstat) = <FILE>;
    close FILE;
    shift @rstat;
    shift @rstat;
    open FILE, '<data/hierachie';
    flock FILE, 2;
    my(@hierachie) = <FILE>;
    close FILE;
    my(@userlevels) = (@hierachie, @rstat);
    foreach $_ (@userlevels) {
        my($name, $level) = split(/<;/, $_, 3);
        if ($name eq $alias) {
            chomp $level;
            return $level;
        }
    }
    return 0;
}
sub hierachie_rm {
    my $alias = shift @_;
    my @return;
    open FILE, '<data/hierachie';
    my(@hierachie) = <FILE>;
    close FILE;
    foreach $_ (@hierachie) {
        push @return, $_ unless /^$alias<;/;
    }
    open FILE, '>data/hierachie';
    print FILE @return;
    close FILE;
}
sub hierachie_rm_rstat {
    my($alias, $room) = @_;
    my @return;
    return unless open FILE, "<data/online/rstat/$room";
    my(@hierachie) = <FILE>;
    close FILE;
    foreach $_ (@hierachie) {
        push @return, $_ unless /^$alias<;/;
    }
    open FILE, ">data/online/rstat/$room";
    print FILE @return;
    close FILE;
}
sub hierachie_add {
    my($alias, $level) = @_;
    &hierachie_rm($alias);
    open FILE, '>>data/hierachie';
    print FILE "$alias<;$level\n";
    close FILE;
}
sub hierachie_add_rstat {
    my($alias, $level, $room) = @_;
    &hierachie_rm_rstat($alias, $room);
    return unless open FILE, ">>data/online/rstat/$room";
    print FILE "$alias<;$level\n";
    close FILE;
}
sub prove_away {
    my($alias, $room) = @_;
    return unless open AWAY, "<data/online/rstat/$room.away";
    @away = <AWAY>;
    close AWAY;
    my $name, $away;
    foreach $_ (@away) {
        if (/^$alias.*/) {
            ($name, $away) = split(/<; /, $_, 3);
            chomp $away;
            return $away;
        }
    }
}
1;
