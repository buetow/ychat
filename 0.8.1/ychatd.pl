#!/usr/bin/perl
use CGI;
$q = new CGI;

use config;
use modules::commands;
use modules::changeroom;
use modules::divcom;
use modules::hierachie;
use modules::htdocs;
use modules::input;		
use modules::loggedin;
use modules::online;
use modules::postmessage;
use modules::shared;
use modules::start;		
use modules::statistic;
use modules::stream;
use modules::webchat;
use IO::Socket;
use Socket;
use POSIX;

$SIG{'CHLD'} = 'IGNORE';

$mainsocket = 'IO::Socket::INET'->new('LocalHost', "$localhost", 'LocalPort', $localport, 'Porto', 'tcp', 'Listen', $limit + 10, 'Reuse', 1);
die "Kann Server-Socket nicht erzeugen: $!\n" unless $mainsocket;

print "yChat wurde auf $localhost:$localport gestartet...\n";

$STARTIME = time;

#Debugging only
#open AWAKE, ">data/online/awake/-$$-";
#print AWAKE "$STARTIME - Daemon";
#close AWAKE;

while ($client = $mainsocket->accept) {
    print $client "HTTP/1.1 200 OK\nContent-type:text/html\n\n";

    ++$HITS;
    $pid = fork;
    if ($pid == 0) {
	my @reqheader;

	$rv = $client->recv($reqheader[0], POSIX::BUFSIZ, 0);
	@reqheader = split "\n", $reqheader[0];

        my $requeststring;
        my(@tmp) = split(/\s+/, $reqheader[0], 0);

            if ($tmp[0] eq 'GET') {
                $requeststring = $tmp[1];

	    } else {
		$requeststring = 'chat?'.$reqheader[-1];
	    }

	    #Debug only
	    #open AWAKE, ">data/online/awake/$$";
	    #print AWAKE $requeststring;
            #close AWAKE;
          
            @tmp = split(/\?/, $requeststring, 0);
            @tmp = split(/&/, $tmp[1], 0);
            my %variables;

            foreach $_ (@tmp) {
                my(@tmp) = split(/=/, $_, 0);
                next if $tmp[0] eq 'admin' or $tmp[0] eq 'mogeladmin';
                $tmp[0] =~ s[/][];
                $variables{$tmp[0]} = &url_decode($tmp[1]);
            }

            foreach $_ (keys %variables) {
		$variables{$_} =~ s/'/\\'/g;
                my $eval = "\$$_ = '" . $variables{$_} . "';";
               # print "$eval\n";
		eval $eval;
            }
	    *STD = *STDOUT;
            *STDOUT = $client;
            if ($request) {
                &secure_checkid($alias, $tmpid) if $request != 'start';
                eval "&$request";
            }
            else {
                &start;
            }
	    &del_awake();
            $client->shutdown(2);
            exit 0;
    }
}
