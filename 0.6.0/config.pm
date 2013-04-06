# yChat - Copyright by Paul C. Bütow

########################### Dieser Teil bestimmt die Standart-Variabeln.
##STANDART-CONFIGURATION:## (CSS, Farben, Version etc.)
###########################
$datum = "22.02.01";
$version = "0.6.0";
$title = "yChat [$version]";
$gfxpath = "../../yChat";
$style = <<ENDCSS;
<style type="text/css">
 body { background-color: #005146 }
 body.blank { background-color: #000000 }
 body.online { background-color: #000000 }
 div { font-family: arial, geneva, verdana, helvetiva;  font-size: 12px; color: #ffffff }
 div.b { font-weight: bold; color: #ffa500 }
 a { color: #ffffef; }
 a:hover { color: #ffffff; }
 p {  font-family:verdana, arial, geneva, helvetica, sans-serif;  color:#FFFFFF;  font-size:12px; }
</style>
<style type="text/css" media="all">
 a { text-decoration: none; }
 a:hover { text-decoration:underline; }
 input { border:2px solid #000000; font-size:12px; color:#000000; background-color: #ffffff; height:23px; padding:2px;}
 select { border:2px solid #000000; font-family:arial, verdana, helvetica; font-size:11px; color:#000000; height:21px; padding:2px;}
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
  while(<FILE2PRINT>) {
   print "$_\n";
  }
 close FILE2PRINT;
}

sub start_html { # Der HEADER einer HTML-Datei
 print "<html><head><title>$title -  $_[0]</title>$_[2]$style</head>";
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
 my @rooms = $room;
 @rooms = ($room,$secroom) if ($room ne $secroom);
 foreach $raum (@rooms) {
  open(MSGFILE,">>data/msgs/$raum");
   print MSGFILE "!<;".time."<;!<;!<;$msg2post<;\n";
  close MSGFILE;
  opendir(PID,"data/online/pids/$raum");
   my @pids = readdir(PID);
  closedir(PID);
  foreach(@pids) {
   if (-f "data/online/pids/$raum/$_") {
    kill USR1 => $_;
   }
  }
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
    opendir(PID,"data/online/pids/$raum");
     my @pids = readdir(PID);
    closedir(PID);
    foreach(@pids) {
     if (-f "data/online/pids/$raum/$_") {
      kill USR1 => $_;
     }
    }
    goto ENDPRV;
   }
  }
 }
ENDPRV:
}

sub log { # Protokollieren der Nachrichten etc.
 my $msg2log = $_[0];
 my $js;
 &zeit;
 ($msg2log,$js) = split(/<script/, $msg2log);
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

sub error { #  Error-Ausgabe.
 my $error_msg = $_[0];
 &start_html( "Error: ($error_msg)" );
 print $q->div( "Error: ($error_msg)" ),
       $q->end_html;
 open(ERROR,">>data/error");
  flock(ERROR, 2);
  print ERROR "Alias: $alias TempID: $tmpid File. $0 PID: $$ Time: ".time." Message: $error_msg \n";
 close ERROR;
 exit 0;
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
       &rm_alias($benutzer,$raum);
      } else {
       unlink("data/online/rooms/$raum/$benutzer");
      }
      &zeit;
      &post($raum,"<i><font color=ffffff>($hours:$min:$sec) $benutzer hat den Chat verlassen ... </font></i>");
     }
    }
   }
  }
  opendir(PIDS,"data/online/pids/$raum");
   my @pids = readdir(PIDS);
  closedir(PIDS);
  if ($raumleer == 1) { # Falls Raum leer ist => entf.
   rmdir("data/online/rooms/$raum");
   unlink("data/online/rstat/$raum");
   unlink("data/online/rstat/$raum.away");
   unlink("data/msgs/$raum");
   foreach(@pids) {
    if (-f "data/online/pids/$raum/$_") {
     unlink("data/online/pids/$raum/$_");
    }
   }
   rmdir("data/online/pids/$raum");
  } else  {
   foreach(@pids) {
    unless (kill 0 => $_) {
     unlink("data/online/pids/$room/$_");
    }
   }
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
  chomp($rstat[$i]);
  push(@rstat2,$rstat[$i]."\n") if ($rstat[$i] ne $r_alias);
 }
 open (RSTAT,">data/online/rstat/$rstatroom");
  flock(RSTAT, 2);
  print RSTAT @rstat2;
 close RSTAT;
}

sub rm_away { # Benutzer als Raumbesetzer austragen
 my ($a_alias,$rstatroom) = @_;
 open (AWAY,"<data/online/rstat/$rstatroom.away");
  my @away = <AWAY>;
 close AWAY;
 my @away2;
 foreach (@away) {
  my @split = split(/<;/);
  push(@away2, $_) if ($a_alias ne $split[0]);
 }
 open (AWAY,">data/online/rstat/$rstatroom.away");
  print AWAY @away2;
 close AWAY;
}

sub secure_checkid { # TmpID überprüfen
 my ($alias2check,) = @_;
 open(IDENT,"<data/online/ident/$alias2check");
  my @ident = <IDENT>;
 close IDENT;
 if ($ident[1] ne $tmpid) {
  &error("Falsche TempID!");
 }
}

sub hierachie { # Chatter nach SU überprüfen.
 open WA, "<data/wa";
  my @was = <WA>;
 close WA;
 foreach(@was) {
  chomp;
  if ($_ eq $_[0]) {
   return "wa";
  }
 }
 open OW, "<data/ow";
  my @ows = <OW>;
 close OW;
 foreach(@ows) {
  chomp;
  if ($_ eq $_[0]) {
   return "ow";
  }
 }
}

sub prove_besetzer { #  Prüfen, ob Benutzer Raumbesetzerrechte hat
 my ($alias,$room,$method) = @_;
 open DATEI,"<data/online/rstat/$room";
  my @rstat = <DATEI>;
 close DATEI;
 if ($method eq "return_list") {
  my @value;
  for($i=2;$i<=$#rstat;$i++) {
   push @value,$rstat[$i];
  }
  return @value;
 }
 for($i=2;$i<=$#rstat;$i++) {
  chomp($rstat[$i]);
  if ($rstat[$i] eq $alias) {
   return 1;
  }
 }
}

sub prove_away { #  Prüfen, ob Benutzer abgemeldet ist
 my ($a_alias,$room,$method) = @_;
 open(DATEI,"<data/online/rstat/$room.away");
  @away = <DATEI>;
 close DATEI;
 if ($method eq "return_list") {
  my %away;
  foreach(@away) {
   $away{$alias} = split(/<; /);
  }
  return %away;
 }
 my $alias, $away;
 foreach(@away) {
  if (/^$a_alias.*/) {
   ($alias,$away) = split(/<; /);
   chomp($away);
   return $away;
  }
 }
}

# sub debug { open DEBUG,">data/debug"; while(@_) { chomp; print DEBUG "$_\n"; } close DEBUG;}