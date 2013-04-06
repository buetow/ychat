# yChat - Copyright by Paul C. Bütow


########################### Dieser Teil bestimmt die Standart-Variabeln.
##STANDART-CONFIGURATION:## (CSS, Farben, Version etc.)
###########################
$datum = "30.12.2k";
$version = "0.4.3";
$title = "yChat.de ($version)";
$gfxpath = "../../yChat";
$style = <<ENDCSS;
<style rel="stylesheet" type="text/css">
 body { background-color: 005146 }
 body.blank { background-color: 000000 }
 body.online { background-color: 000000 }
 body.log { background-color: 000000 }
 div { font-family: arial, verdana, helvetiva;	font-size: 9pt; color: ffffff }
 div.b { font-weight: bold; color: ffa500 }
 a { color: ffffef }
 a:hover { color: ffffff; text-decoration: none }
</style>
ENDCSS

############### Dieser Teil enthält Programmcode, der an verschiedenen
##SHARED-SUBS## Stellen ausgeführt werden muß und jedem Skript zur
############### Verfügung steht.

sub printfile { # Liest eine externe Datei ein und gibt sie mit print aus
 my ($file2print,$pagetitle,$bodyclass) = @_;
 if ($pagetitle ne "") {
  &start_html($pagetitle,$bodyclass);
 }
 &read_file($file2print);
 @file2print = <DATEI>;
 close DATEI;
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

sub post { # Öffentliche Nachricht im aktuellen Raum $room posten.
 my ($room,$msg2post,$secroom) = @_;
 my @rooms = $room;
if ($room ne $secroom) {
 my @rooms = ($room,$secroom);
}
 foreach(@rooms) {
  my $room2post = $_;
  &list_dir("data/online/rooms/$room2post");
  foreach(@dir) {
   my $alias2post = $_;
   if (-f "data/online/rooms/$room2post/$alias2post") {
    open (DATEI,">>data/msgs/$alias2post") || &error("Konnte $file2write nicht öffnen");
     print DATEI "$msg2post<br>\n";
    close DATEI;
   }
  }
 }
 &log($msg2post) if ($room eq "Cyberbar");
}


sub post_all { # Öffentliche Nachricht in ALLEN Räumen gleichzeitig posten.
 my $msg2post = $_[0];
  &list_dir("data/online/users");
  foreach $alias2post (@dir) {
   if (-f "data/online/users/$alias2post") {
    open (DATEI,">>data/msgs/$alias2post") || &error("Konnte $file2write nicht öffnen");
     print DATEI "$msg2post<br>\n";
    close DATEI;
   }
  }
 &log($msg2post);
}

sub post_prv { # Private Nachricht posten (flüstern).
 my $alias2post = $_[0];
 my $msg2post = $_[1];
 &write_file("data/msgs/$alias2post");
  print DATEI "$msg2post<br>\n";
 close DATEI;
}

sub log { # Protokollieren der Nachrichten etc.
 my ($msg2log) = @_;
 &zeit;
 &write_file("data/logs/$day.$month.$year");
  print DATEI "<br><font color=ffffef><i>($hours:$min:$sec)</i></font> $msg2log\n";
 close DATEI;
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

sub count { # Zeilen in einer Datei Zählen.
 my $file2count = $_[0];
 &read_file($file2count);
 my @datei = <DATEI>;
 close DATEI;
 $number = $#datei+1;
}

sub read_file { # Datei öffnen.
 my $file2read = $_[0];
 if (-e "$file2read") {
  open (DATEI,"<$file2read") || &error("Konnte $file2read nicht öffnen");
 }
}

sub write_file { # Text an eine Datei anhängen.
 my $file2write = $_[0];
 if (-e "$file2write") {
  open (DATEI,">>$file2write") || &error("Konnte $file2write nicht öffnen");
 } else {
  &create_file("$file2write");
  open (DATEI,">>$file2write") || &error("Konnte $file2write nicht öffnen");
 }
}

sub write_file_new { # Datei neu schreiben.
 my $file2write = $_[0];
 if (-e "$file2write") {
  open (DATEI,">$file2write") || &error("Konnte $file2write nicht öffnen");
 } else {
  &create_file("$file2write");
  open (DATEI,">$file2write") || &error("Konnte $file2write nicht öffnen");
 }
}

sub create_file { # Datei erstellen.
 my $file2create = $_[0];
 open (CREATE, ">$file2create") || &error("Konnte $file2create nicht erstellen!");
  print CREATE "";
 close CREATE;
}

sub error { #  Error-Ausgabe.
 my $error_msg = $_[0];
 &start_html( "Error: ($error_msg)" );
 print $q->div( "Error: ($error_msg)" ),
       $q->end_html;
 exit;
}

sub check_online {
 open(PROVE,">data/online/prove");
  print PROVE time;
 close PROVE;
 opendir(RAUMDIR, "data/online/rooms");
  @raumdir = readdir(RAUMDIR);
 closedir(RAUMDIR);
 foreach $raum (@raumdir) {
  opendir(BENUTZERDIR, "data/online/rooms/$raum");
   @benutzerdir = readdir(BENUTZERDIR);
  closedir(BENUTZERDIR);
  my $raumleer= 1;
  foreach $benutzer (@benutzerdir) {
   if (-f "data/online/rooms/$raum/$benutzer") {
   $raumleer = 0;
    open (BENUTZER,"<data/online/rooms/$raum/$benutzer");
     $benutzerstamp = <BENUTZER>;
    close BENUTZER;
    if ($benutzerstamp < (time - 40)) {
     unlink("data/online/$raum/$benutzer");
     open (BENUTZER2,"<data/online/users/$benutzer");
      $benutzerstamp2 = <BENUTZER2>;
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
  }
 }
}

sub rm_alias { # Falls Benutzer offline gegangen ist
 my($benutzer,$raum) = @_;
 unlink("data/online/rooms/$raum/$benutzer");
 unlink("data/online/users/$benutzer");
 unlink("data/online/ident/$benutzer");
 unlink("data/msgs/$benutzer");
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

sub secure_removeid { # Beim ausloggen die IP-Nummer aus dat/ip austragen.
 my $alias2rm = $_[0];
 unlink("data/online/ident/$alias2rm");
}

sub secure_checkid { # TmpID überprüfen
 my ($alias2check) = @_;
 &read_file("data/online/ident/$alias2check");
  my @idfile = <DATEI>;
 close DATEI;
 if ($idfile[1] ne $tmpid) {
  &error("Falsche TempID ! Du bist nicht befugt zu handeln !");
 }
}

sub remove_rooms { # Leere Räume entfernen.
 &list_dir("data/online/rooms");
 foreach(@dir) {
  if (-d "data/online/rooms/$_") {
   &list_dir("data/online/rooms/$_");
   rmdir("data/online/rooms/$_") if ($#dir = 1);
  }
 }
}

sub list_dir { # Beliebigen Verzeichnisbaum einlesen.
 my ($dir,$method) = @_;
 opendir(DIR, $dir);
 if ($method eq undef) {
  @dir = readdir(DIR);
 } elsif ($method = 2) {
  @dir2 = readdir(DIR);
 }
 closedir(DIR);
}

sub hirachie { # Chatter nach Sonderrechte überprüfen.
 my $alias2prove = $_[0];
 $hirachie = undef;
 &read_file("data/wa");
  @was = <DATEI>;
 close DATEI;
 foreach(@was) {
  if ($_ eq $alias2prove || $_ eq "$alias2prove\n") {
   $hirachie = "wa";
  }
 }
 &read_file("data/ow");
  @ows = <DATEI>;
 close DATEI;
 if ($hirachie ne "wa" || $_ eq "$alias2prove\n") {
  foreach(@ows) {
   if ($_ eq $alias2prove || $_ eq "$alias2prove\n") {
    $hirachie = "ow";
   }
  }
 }
}

$| = 1; # Cache von Perl ausschalten