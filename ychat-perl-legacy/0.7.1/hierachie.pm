sub hierachie {
 my ($alias,$room) = @_;

 open FILE, "<data/online/rstat/$room";
  my @rstat = <FILE>;
 close FILE;
 shift @rstat;
 shift @rstat;

 open FILE, "<data/hierachie";
  my @hierachie = <FILE>;
 close FILE;

 my @userlevels = (@hierachie,@rstat);
 foreach (@userlevels) {
  my ($name, $level) = split "<;";

  if ($name eq $alias) {
   chomp $level;
   return $level;
  }
 }
}

sub hierachie_rm {
 my $alias = shift;
 my @return;

 open FILE, "<data/hierachie";
  my @hierachie = <FILE>;
 close FILE;

 foreach (@hierachie) {
  push @return, $_
   unless (/^$alias<;/);
 }

 open FILE, ">data/hierachie";
  print FILE @return;
 close FILE;
}

sub hierachie_rm_rstat {
 my ($alias,$room) = @_;
 my @return;

 open FILE, "<data/online/rstat/$room" or return;
  my @hierachie = <FILE>;
 close FILE;

 foreach (@hierachie) {
  push @return, $_
   unless (/^$alias<;/);
 }

 open FILE, ">data/online/rstat/$room";
  print FILE @return;
 close FILE;
}


sub hierachie_add {
 my ($alias,$level) = @_;

 &hierachie_rm($alias);

 open FILE, ">>data/hierachie";
  print FILE "$alias<;$level\n";
 close FILE;
}

sub hierachie_add_rstat {
 my ($alias,$level,$room) = @_;

 &hierachie_rm_rstat($alias,$room);

 open FILE, ">>data/online/rstat/$room" or return;
  print FILE "$alias<;$level\n";
 close FILE;
}

sub prove_away { #  Prüfen, ob Benutzer abgemeldet ist
 my ($alias,$room) = @_;

 open AWAY,"<data/online/rstat/$room.away" or return;
  @away = <AWAY>;
 close AWAY;

 my $name, $away;
 foreach(@away) {

  if (/^$alias.*/) {
   ($name,$away) = split(/<; /);
   chomp($away);
   return $away;
  }
 }
}


1;
