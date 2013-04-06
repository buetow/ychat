#!/usr/bin/perl
# yChat - Copyright by Paul C. Bütow
# Dieses yChat-System wurde von Paul C. Bütow alias Snooper eigenhändig programmiert.
# Es darf kostenlos verwendet und manipuliert werden, allerdings darf dieses System nicht
# Weiterverkauft werden! Der Hinweis der bei /about erscheind, darf ebenso nicht entfernt werden!
# Für Fragen/Anregungen/Verbesserungsvorschläge bitte eine E-Mail an untenstehende Adresse senden!

# Programmierer: Paul C. Bütow
# Seine Homepage: http://buetow.info
# Seine E-Mail Adresse: mail@buetow.de
# Projektname: yChat / 2000 - 2001
# Programmiersprache: Perl 5.6
# Platttform: Linux
# Die Adresse des Original-yChats: http://www.yChat.de


use CGI;
$q = new CGI;

# Module einbinden
use config;
use commands;
use changeroom;
use start;
use shared;
use input;
use main1;
use main2;
use online;
use htdocs;
use stream;
use stat;
use loggedin;
use hierachie;
use IO::Socket;

# Forkprozzesse killen, wenn beendet
$SIG{CHLD} = 'IGNORE';

$mainsocket = IO::Socket::INET->new(LocalHost=>"$localhost",
                                LocalPort=>$localport,
				Porto=> 'tcp',
				Listen=> ($limit+10),
				Reuse=>1,);

die "Kann Server-Socket nicht erzeugen: $!\n"
 unless ($mainsocket);

print "yChat wurde auf $localhost:$localport gestartet...\n";
$STARTIME = time;

while ($client = $mainsocket->accept()) {
 $HITS++;
 $pid = fork();

 if ($pid == 0) {
  print $client "HTTP/1.1 200 OK\nContent-type:text/html\n\n";
  while (defined ($buffer = <$client>)) {
   my $requeststring;

   my @tmp = split ' ', $buffer;

   # Falls GET request
   if ($tmp[0] eq "GET") {
    $requeststring = $tmp[1];

   # Falls POST request
   } else {
    print "POST\n";
   }

    @tmp = split '\?', $requeststring;
    @tmp = split '&', $tmp[1];

    my %variables;
    foreach(@tmp) {
     my @tmp = split "=";

     next
      if ($tmp[0] eq "admin" || $tmp[0] eq "mogeladmin");

     $tmp[0] =~ s#/##;
     $variables{$tmp[0]} = &url_decode( $tmp[1] );
    }

    eval "my \$$_"
     foreach ( keys %variables );

    eval "\$$_ = '".$variables{$_}."'"
     foreach ( keys %variables );

    # Ausgabe umleiten
    STDOUT = $client;

    if ($request) {
     &secure_checkid($alias,$tmpid) if ($request != "start");
     eval "\&$request";

    } else {
     &start;
    }

   $client->shutdown(2);
   exit(0);
  }
 }
}


