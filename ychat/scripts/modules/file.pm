sub dopen
{
 my $shift = shift;
 opendir DIR, $shift or die "$shift: $!\n";
 my @dir = readdir(DIR);
 closedir DIR;
 return @dir;
}

sub fopen
{
 my $shift = shift;
 open FILE, $shift or die "$shift: $!\n";
 my @file = <FILE>; 
 close FILE;
 return @file;
}

sub fwrite
{
 my $shift = shift;
 my @file = @_;
 open FILE, ">$shift" or die "$shift: $!\n";
 print FILE @file;
 close FILE;
}

1;
