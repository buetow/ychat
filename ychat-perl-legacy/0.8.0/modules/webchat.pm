sub webchat {
   my @rooms = &diropen('data/online/rooms');
   my $output;

   foreach my $room (@rooms) {
	next if ($room =~ /\./);
   	my @users = &diropen("data/online/rooms/$room") or print "$!\n";

	foreach my $user (@users) {
		next if ($user =~ /\./);
		open FILE, "data/online/rooms/$room/$user";
		my $timestamp = <FILE>;
		close FILE;
		chomp $timestamp;

		$output = $output . "($user)($timestamp)($room)\n";
	}
   }

  print $output;
}
1;
