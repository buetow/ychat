#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen


#####################################
##WELCHE SUB MUß AUSGEFÜHRT WERDEN?##
#####################################
if ($var eq "help_html") {
 &printfile("help.sno","Alle Chatbefehle");
} elsif ($var eq "menu_html") {
 &menu_html;
} elsif ($var eq "werda_html") {
 &werda_html;
} elsif ($var eq "logs_html") {
 &logs_html;
} elsif ($var eq "view_logs_html") {
 &secure_checkid($alias);
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  &printfile($tmp,$tmp,"log");
 }
} elsif ($var eq "superuser_html") {
 &secure_checkid($alias);
 &superuser_html;
} else {
 print
  $q->start_html(-bgcolor=>"000000"),
  $q->end_html;
}

sub menu_html {
 if ($tmp eq "quit") {
  print
   $q->script( { -language=>"JavaScript" }, "window.open(\"index.pl?tmp=quit\",\"_top\")" );
  sleep 1;
  &remove_online($alias);
  &secure_removeid($alias);
  unlink("data/msgs/$alias");
 }
 &start_html("Menü","menu");
print <<END;
 <center>
  <div>
   [<a href="htdocs.pl?var=werda_html&alias=$alias&room=$room&col=$col&secol=$secol&&tmp=$tmp&tmpid=$tmpid" target="werda">Wer ist da?</a>]
   [<a href="main2.pl?var=edit_profile_html&alias=$alias&tmpid=$tmpid" target="ProfilAendern">Profil ändern</a>]
   [<a href="main2.pl?var=edit_options_html&alias=$alias&tmpid=$tmpid" target="main">Optionen</a>]
   [<a href="main2.pl?var=list_alias_html&alias=$alias&tmpid=$tmpid" target="Benutzerliste">Benutzer</a>]
   [<a href="htdocs.pl?var=help_html&tmpid=$tmpid" target="Befehle">Befehle</a>]
   [<a href="main2.pl?var=gb_html&alias=$alias&tmpid=$tmpid" target="Board">Board</a>]
   [<a href="htdocs.pl?var=superuser_html&alias=$alias&tmpid=$tmpid" target="Hirachie">Superuser</a>]
   [<a href="htdocs.pl?var=logs_html&alias=$alias&tmpid=$tmpid" target="Logdateien">Logs</a>]
   [<a href="push.pl?alias=$alias&room=$room&var=clear_screen&tmpid=$tmpid" target="main">Weiterchatten</a>]
   [<a href="htdocs.pl?var=menu_html&tmp=quit&alias=$alias&tmpid=$tmpid">Verlassen</a>]
  </div>
 </center>
</body>
</html>
END
}


sub werda_html { # Anzeige, wer eingelogt ist.
 &start_html("Wer ist da?");
 print <<END;
<div>
 $alias, schön daß Du hier bist :-)
</div>
<hr>
<div>
 Folgende Personen befinden sich im Chat:
</div>
<br>
<div>
END
 &list_dir("data/online/rooms");
 foreach(@dir) {
  if (-e "data/online/rooms/$_" && $_ ne ".." && $_ ne ".") {
   my $room = $_;
   &read_file("data/online/rstat/$room");
    my @rstat = <DATEI>;
   close DATEI;
   my $door = "[<font color=red>Abgesperrt!</font>]" if ($rstat[0] eq "1\n");
   &list_dir("data/online/rooms/$room",2);
   local $nr = $#dir2-1;
   print $q->b( $_ ),
         " $door ($nr) Raumthema: $rstat[1]",
         $q->br;
   foreach(@dir2) {
    if ($_ ne ".." && $_ ne ".") {
     print
      $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid" }, "$_"), " ";
    }
   }
   print $q->br, $q->br;
  }
 }
 print
  "</div>",
  $q->end_html;
}

sub logs_html {
 &start_html("Logdateien");
 print <<END;
<div>
 Hier befinden sich alle Logdateien
</div>
<hr>
END
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  print $q->div( "Folgende Logdateien des Raumes \"Cyberbar\" können nur von Wächtern und Oberwächtern gelesen werden (Sortiert nach A-Z, a-z):" ),
        $q->br;
  &list_dir("data/logs");
  my @sortedir = sort(@dir);
  foreach(@sortedir) {
   if (-f "data/logs/$_") {
    print $q->a( { -href=>"htdocs.pl?alias=$alias&var=view_logs_html&tmp=data/logs/$_&tmpid=$tmpid",
  	           -target=>"_new" },
 	  "$_" ),
 	  " ";
   }
  }
 } else {
  print $q->div( "Logdateien können nur von Oberwächtern (OW) und von Wächtern (WÄ) gelesen werden. Du bist jedoch ein ",
  		  "\"Normaler Chatter\". Wächter können alles nachlesen, was öffentlich gechattet wurde." ),
  $q->hr;
  &printfile("data/htdocs/logs_html");
 }
 print
  $q->end_html;
}

sub superuser_html { # Zeigt an, wer   Superuser ist
 &start_html("Superuser");
 print "<div>Hallo $alias. Du bist als ";
 &hirachie($alias);
 if ($hirachie eq "ow") {
  print
   $q->b( "Oberwächter" ),
   " (OW) eingetragen.</div>",
   $q->hr;
 } elsif ( $hirachie eq "wa") {
  print
   $q->b( "Wächter" ),
   " (WÄ) eingetragen.</div>",
   $q->hr;
 } else {
  print
  $q->b( "normaler Chatter" ),
  " (NC) eingetragen.</div>",
  $q->hr;
 }

 my $nrow = $#ows+1;
 print
  "<div>",
  $q->b( "OW" ),
  "s ($nrow Stück)";
 foreach(@ows) {
  print
   $q->font( { -color=>"FFA500" },
   	" ".$_);
 }
 my $nrwa = $#was+1;
  print
  "</div>",
  $q->hr,
  "<div>",
  $q->b( "WÄ" ),
  "s ($nrwa Stück)";
 foreach(@was) {
   print
   $q->font( { -color=>"FFA500" },
   	" ".$_);
 }
print <<END;
<hr>
 Die jeweiligen Fähigkeiten der Oberwächter (OW) und der Wächter (WÄ) sind unter
 "<a href="htdocs.pl?var=help_html&tmpid=$tmpid">Chat-Befehle</a>"
 dokumentiert.
</div>
</body>
</html>
END
}