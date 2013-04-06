# yChat - Copyright by Paul C. Bütow

########################### Dieser Teil bestimmt die Standart-Variabeln.
##STANDART-CONFIGURATION:## (CSS, Farben, Version etc.)
###########################
$alias = $q->param("alias");
$var = $q->param("var");
$room = $q->param("room");
$col = $q->param("col");
$secol = $q->param("secol");
$msg = $q->param("msg");
$tmpid = $q->param("tmpid");
$tmp = $q->param("tmp");
$id = $q->param("id");
$room = "Cyberbar" if ($room eq "");
$col = "ffffef" if ($col eq "");
$secol = "ffffef" if ($secol eq "");
$datum = "25.12.2k";
$version = "0.4.2";
$title = "yChat.de ($version)";
$gfxpath = "../../yChat/";
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
 if ($_[1] ne "") {
  print "<body class=$_[1]>";
 } else {
   print "<body>";
 }
}

sub post { # Öffentliche Nachricht im aktuellen Raum $room posten.
 my ($msg2post,$secroom) = @_;
 my @rooms = ($room,$secroom);
 foreach(@rooms) {
  my $room2post = $_;
  &list_dir("data/online/rooms/$room2post");
  foreach(@dir) {
   my $alias2post = $_;
   if (-f "data/online/rooms/$room2post/$alias2post") {
    &write_file("data/msgs/$alias2post");
     print DATEI "$msg2post<br>\n";
    close DATEI;
   }
  }
 }
 &log("$msg2post") if ($room eq "Cyberbar");
}


sub post_all { # Öffentliche Nachricht in ALLEN Räumen gleichzeitig posten.
 my $msg2post = $_[0];
  &list_dir("data/online/users");
  foreach(@dir) {
   my $alias2post = $_;
   if (-f "data/online/users/$alias2post") {
    &write_file("data/msgs/$alias2post");
     print DATEI "$msg2post<br>\n";
    close DATEI;
   }
  }
 &log("$msg2post");
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

sub remove_online { # Chatter entfernen
 my ($alias,$room,$method) = @_;
 unlink("data/online/users/$alias");
 unlink("data/online/rooms/$room/$alias") if ($room ne undef);
 unlink("data/online/ident/$alias");
 unlink("data/msgs/$alias");
 &rm_rooms;
 &zeit;
 if ($method eq undef) {
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias verläßt den Chat ...</font></i>");
 } elsif ($method ne "kick") {
  &post("<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500> $_ hat den Chat verlassen ... </font></i>");
 }
 &rm_rstat($room,$alias) if ($room ne undef);
}

sub rm_alias { # Chatter aus dem System entfernen, dessen Timestamps älter als 60 Sekunden sind
 my ($dir2rm,$rstatroom) = @_;
 &list_dir($dir2rm,2);
 foreach(@dir2) {
  if (-f "$dir2rm/$_" && $_ ne undef) {
   &read_file("$dir2rm/$_");
    my $timestamp = <DATEI>;
   close DATEI;
   if ($timestamp < (time - 60)) {
    if ($dir2rm eq "data/online/users") {
     &remove_online($_,undef,1);
    } else {
     unlink("$dir2rm/$_");
     &rm_rstat($rstatroom,$_) if ($rstatroom ne undef);
    }
   }
  }
 }
}

sub rm_rooms { # Leere Räume aus dem System entfernen
 &list_dir("data/online/rooms");
 foreach(@dir) {
  if (-d "data/online/rooms/$_") {
   &rm_alias("data/online/rooms/$_",$_);
   &list_dir("data/online/rooms/$_",2);
   if ($#dir2 = 1) {
    rmdir("data/online/rooms/$_");
   }
  }
 }
}

sub rm_rstat { # Benutzer als Raumbesetzer austragen
 my ($rstatroom,$r_alias) = @_;
  &read_file("data/online/rstat/$rstatroom");
   my @rstat = <DATEI>;
  close DATEI;
  my $newfile = $rstat[0].$rstat[1];
  for ($i=2;$i<=$#rstat;$i++) {
   $newfile = $newfile."\n".$rstat[$i] if ($rstat[$i] ne $r_alias."\n");
  }
 write_file_new("data/online/rstat/$rstatroom");
  print DATEI $newfile;
 close DATEI;
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