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
$SIG{'CHLD'} = 'IGNORE';
$mainsocket = 'IO::Socket::INET'->new('LocalHost', "$localhost", 'LocalPort', $localport, 'Porto', 'tcp', 'Listen', $limit + 10, 'Reuse', 1);
die "Kann Server-Socket nicht erzeugen: $!\n" unless $mainsocket;
print "yChat wurde auf $localhost:$localport gestartet...\n";
$STARTIME = time;
while ($client = $mainsocket->accept) {
    ++$HITS;
    $pid = fork;
    if ($pid == 0) {
        print $client "HTTP/1.1 200 OK\nContent-type:text/html\n\n";
        while (defined($buffer = <$client>)) {
            my $requeststring;
            my(@tmp) = split(/\s+/, $buffer, 0);
            if ($tmp[0] eq 'GET') {
                $requeststring = $tmp[1];
            }
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
                eval "\$$_ = '" . $variables{$_} . q['];
            }
            *STDOUT = $client;
            if ($request) {
                &secure_checkid($alias, $tmpid) if $request != 'start';
                eval "&$request";
            }
            else {
                &start;
            }
            $client->shutdown(2);
            exit 0;
        }
    }
}
