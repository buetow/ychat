# yChat - Copyright by Paul C. Bütow

########################### Dieser Teil bestimmt die Standart-Variabeln.
##STANDART-CONFIGURATION:## (CSS, Farben, Version etc.)
###########################
$datum = "18.01.01";
$version = "0.4.5a";
$title = "yChat [$version]";
$gfxpath = "../../yChat";
$style = <<ENDCSS;
<style rel="stylesheet" type="text/css">
 body { background-color: #005146 }
 body.blank { background-color: #000000 }
 body.online { background-color: #000000 }
 div { font-family: arial, verdana, helvetiva;	font-size: 9pt; color: #ffffff }
 div.b { font-weight: bold; color: #ffa500 }
 a { color: #ffffef }
 a:hover { color: #ffffff; text-decoration: none }
</style>
ENDCSS

############### Dieser TeFil enthält Programmcode, der an verschiedenen
##SHARED-SUBS## Stellen ausgeführt werden muß und jedem Skript zur
############### Verfügung steht.

sub printfile { # Liest eine externe Datei ein und gibt sie mit print aus
 my ($file2print,$pagetitle,$bodyclass) = @_;
 if ($pagetitle ne "") {
  &start_html($pagetitle,$bodyclass);
 }
 open(FILE2PRINT,"<$file2print");
  @file2print = <FILE2PRINT>;
 close FILE2PRINT;
 print @file2print;
}


sub start_html { # Der HEADER einer HTML-Datei
print "<html><head>";
print "<meta http-equiv=\"refresh\" content=\"20\" URL=\"online.pl?room=$room\">" if ($_[1] eq "online");
print <<END;
  <title>
   $title -  $_[0]
  </title>
  $_[2]
  $style
 </head>
END
 if ($_[1] eq "start") {
   print "<body onload=\"document.login.alias.focus();\">";
 } elsif ($_[1] ne "") {
  print "<body class=$_[1]>";
 } else {
   print "<body>";
 }
}

sub post { # Öffentliche Nachricht posten.
 my ($room,$msg2post,$secroom) = @_;
 local @rooms = $room;
 @rooms = ($room,$secroom) if ($room ne $secroom);
 foreach(@rooms) {
  open(MSGFILE,">>data/msgs/$_");
   print MSGFILE "!<;".time."<;!<;!<;$msg2post<;\n";
  close MSGFILE;
 }
 &log($msg2post) if ($room eq "Cyberbar");
}

sub post_prv { # Private Nachricht posten (flüstern).
 my ($alias2post,$msg2post) = @_;
 opendir(DIR,"data/online/rooms");
  my @dir = readdir(DIR);
 closedir(DIR);
 foreach $raum (@dir) {
  opendir(DIR,"data/online/rooms/$raum");
   my @chatter = readdir(DIR);
  closedir(DIR);
  foreach $chatter (@chatter) {
   if ($chatter eq $alias2post) {
    open(MSGFILE,">>data/msgs/$raum");
     print MSGFILE "$alias2post<;".time."<;!<;!<;$msg2post<;\n";
    close MSGFILE;
    goto ENDPRV;
   }
  }
 }
ENDPRV:
}

sub log { # Protokollieren der Nachrichten etc.
 local $msg2log = $_[0];
 &zeit;
 open(LOG,">>data/logs/$day.$month.$year");
  print LOG "<br><font color=ffffef><i>($hours:$min:$sec)</i></font> $msg2log\n";
 close LOG;
}


sub zeit { # Aktuelles Datum und Uhrzeit bestimmen.
 ($sec, $min, $hours, $day, $month, $year) = (localtime)[0..5];
 $month += 1;
 $hours = "0".$hours if ($hours < 10);
 $min = "0".$min  if ($min < 10);
 $sec = "0".$sec  if ($sec < 10);
 $day = "0".$day  if ($day < 10);
 $month = "0".$month if ($month < 10);
 $year = $year  - 100;
 if ($year < 10) {
  $year = "200".$year;
 } else {
  $year = "20".$year;
 }
}

sub read_file { # Datei öffnen.
 my $file2read = $_[0];
 if (-e "$file2read") {
  open (DATEI,"<$file2read") || &error("Konnte $file2read nicht öffnen");
 }
}

sub error { #  Error-Ausgabe.
 my $error_msg = $_[0];
 &start_html( "Error: ($error_msg)" );
 print $q->div( "Error: ($error_msg)" ),
       $q->end_html;
 open(ERROR,">>data/error");
  print ERROR $error_msg."\n";
 close ERROR;
 exit;
}

sub check_online { # Auf alte Räume und Chatter prüfen und ggf. entfernen.
 open(PROVE,">data/online/prove");
  print PROVE time;
 close PROVE;
 opendir(RAUMDIR, "data/online/rooms");
  my @raumdir = readdir(RAUMDIR);
 closedir(RAUMDIR);
 foreach $raum (@raumdir) {
  opendir(BENUTZERDIR, "data/online/rooms/$raum");
   my @benutzerdir = readdir(BENUTZERDIR);
  closedir(BENUTZERDIR);
  my $raumleer= 1;
  foreach $benutzer (@benutzerdir) {
   if (-f "data/online/rooms/$raum/$benutzer") {
   $raumleer = 0;
    open (BENUTZER,"<data/online/rooms/$raum/$benutzer");
     my $benutzerstamp = <BENUTZER>;
    close BENUTZER;
    if ($benutzerstamp < (time - 40)) {
     unlink("data/online/$raum/$benutzer");
     open (BENUTZER2,"<data/online/users/$benutzer");
      my $benutzerstamp2 = <BENUTZER2>;
     close BENUTZER2;
     if ($benutzerstamp2 < (time - 40)) {
      if ($benutzer ne $alias) {
       &rm_alias($benutzer,$raum); # ohne $raum/benutzer entf.
      } else {
       unlink("data/online/rooms/$raum/$benutzer");
      }
      &zeit;
      &post($raum,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$benutzer hat den Chat verlassen ... </font></i>");
     }
    }
   }
  }
  if ($raumleer == 1) { # Falls Raum leer ist => entf.
   rmdir("data/online/rooms/$raum");
   unlink("data/online/rstat/$raum");
   unlink("data/msgs/$raum");
  }
 }
}

sub rm_alias { # Falls Benutzer offline gegangen ist
 my($benutzer,$raum) = @_;
 unlink("data/online/rooms/$raum/$benutzer");
 unlink("data/online/users/$benutzer");
 unlink("data/online/ident/$benutzer");
 &rm_rstat($benutzer,$raum);
}

sub rm_rstat { # Benutzer als Raumbesetzer austragen
 my ($r_alias,$rstatroom) = @_;
 open (RSTAT,"<data/online/rstat/$rstatroom");
  my @rstat = <RSTAT>;
 close RSTAT;
 my @rstat2 = ($rstat[0],$rstat[1]);
 for ($i=2;$i<=$#rstat;$i++) {
  push(@rstat2,$rstat[$i]) if ($rstat[$i] ne $r_alias."\n");
 }
 open (RSTAT,">data/online/rstat/$rstatroom");
  print RSTAT @rstat2;
 close RSTAT;
}


sub secure_checkid { # TmpID überprüfen
 my ($alias2check) = @_;
 open(IDENT,"<data/online/ident/$alias2check");
  my @ident = <IDENT>;
 close IDENT;
 if ($ident[1] ne $tmpid) {
  &error("Falsche TempID ! Du bist nicht befugt zu handeln !");
 }
}

sub hierachie { # Chatter nach Sonderrechte überprüfen.
 my $alias2prove = $_[0];
 $hierachie = undef;
 open(WA,"<data/wa");
  @was = <WA>;
 close WA;
 foreach $wa (@was) {
  if ($wa eq $alias2prove || $wa eq "$alias2prove\n") {
   $hierachie = "wa";
  }
 }
 open(OW,"<data/ow");
  @ows = <OW>;
 close OW;
 if ($hierachie ne "wa" || $_ eq "$alias2prove\n") {
  foreach $ow (@ows) {
   if ($ow eq $alias2prove || $ow eq "$alias2prove\n") {
    $hierachie = "ow";
   }
  }
 }
}

$| = 1; # Cache von Perl ausschalten