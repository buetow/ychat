#!/usr/bin/perl -w

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen


#####################################
##WELCHE SUB MU� AUSGEF�HRT WERDEN?##
#####################################
if ($var eq "help_html") {
 &printfile("data/htdocs/help_html","Alle Chatbefehle");
} elsif ($var eq "menu_html") {
 &menu_html;
} elsif ($var eq "werda_html") {
 &werda_html;
} elsif ($var eq "logs_html") {
 &logs_html;
} elsif ($var eq "view_sulogs_html") {
 &hirachie($alias);
 if ($hirachie eq "ow") {
  &start_html("Oberw�chter-Logdateien");
  print "<div><a href=\"push.pl?alias=$alias&tmp=$tmp&var=push_logs_html\">Push</a><hr>";
  &printfile($tmp);
  print "</div></body></html>";
 }
} elsif ($var eq "view_logs_html") {
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  &printfile($tmp,$tmp,"log");
 }
} elsif ($var eq "superuser_html") {
 &superuser_html;
} elsif ($var eq "bar_html") {
 print
  $q->start_html(-bgcolor=>"ffa500"),
  $q->end_html;
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
  &secure_removeip($alias);
  unlink("data/msgs/$alias");
 }
 &start_html("Men�","menu");
print <<END;
 <center>
  <div>
   [<a href="htdocs.pl?var=werda_html&alias=$alias&room=$room&col=$col&secol=$secol&&tmp=$tmp" target="werda">Wer ist da?</a>]
   [<a href="main2.pl?var=edit_profile_html&alias=$alias" target="ProfilAendern">Profil �ndern</a>]
   [<a href="main2.pl?var=edit_options_html&alias=$alias" target="main">Optionen</a>]
   [<a href="main2.pl?var=list_alias_html&alias=$alias" target="Benutzerliste">Benutzer</a>]
   [<a href="htdocs.pl?var=help_html" target="Befehle">Befehle</a>]
   [<a href="main2.pl?var=gb_html&alias=$alias" target="Board">Board</a>]
   [<a href="htdocs.pl?var=superuser_html&alias=$alias" target="Hirachie">Superuser</a>]
   [<a href="htdocs.pl?var=logs_html&alias=$alias" target="Logdateien">Logs</a>]
   [<a href="push.pl?alias=$alias&room=$room&var=clear_screen" target="main">Weiterchatten</a>]
   [<a href="htdocs.pl?var=menu_html&tmp=quit&alias=$alias">Verlassen</a>]
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
 $alias, sch�n da� Du hier bist :-)
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
   &list_dir("data/online/rooms/$room",2);
   local $nr = $#dir2-1;
   print $q->b( $_ ),
         "($nr)",
         $q->br;
   foreach(@dir2) {
    if ($_ ne ".." && $_ ne ".") {
     print
      $q->a( { -href=>"main2.pl?var=alias_html&alias=$alias&tmp=$_" }, "$_"), " ";
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
 &secure_checkip($alias);
 &start_html("Logdateien");
 print <<END;
<div>
 Hier befinden sich alle Logdateien
</div>
<hr>
END
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  if ($hirachie eq "ow") {
   print $q->div( "Folgende Logdateien k�nnen nur von Oberw�chtern (OW) gelesen werden" );
   &printfile("data/htdocs/logs_html");
   &list_dir("data/logs/ow");
   my @sortedir = sort(@dir);
   foreach(@sortedir) {
    if (-f "data/logs/ow/$_") {
     print $q->a( { -href=>"htdocs.pl?var=view_sulogs_html&alias=$alias&tmp=data/logs/ow/$_",
     		    -target=>$_},
 	   "$_" ).
 	   " ",
    }
   }
  print $q->hr;
  }
  print $q->div( "Folgende Logdateien k�nnen nur von W�chtern und Oberw�chtern gelesen werden (Sortiert nach A-Z, a-z):" ),
        $q->br;
  &list_dir("data/logs");
  my @sortedir = sort(@dir);
  foreach(@sortedir) {
   if (-f "data/logs/$_") {
    print $q->a( { -href=>"htdocs.pl?alias=$alias&var=view_logs_html&tmp=data/logs/$_",
  	           -target=>"_new" },
 	  "$_" ),
 	  " ";
   }
  }
 if ($hirachie eq "wa") {
   print $q->hr,
   	 $q->div( "Oberw�chter k�nnen zus�tzlich die Systemlogdateien lesen. Du bist allerdings ein \"W�chter\" und ",
   		  " bist nicht dazu berechtigt " ),
         $q->br;
   &printfile("data/htdocs/logs_html");
 }
 } else {
  print $q->div( "Logdateien k�nnen nur von Oberw�chtern (OW) und von W�chtern (W�) gelesen werden. Du bist jedoch ein ",
  		  "\"Normaler Chatter\". W�chter k�nnen alles nachlesen, was �ffentlich gechattet wurde. Oberw�chter ",
  		  "k�nnen folgende Systemlogdateien zus�tzlich lesen:" ),
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
   $q->b( "Oberw�chter" ),
   " (OW) eingetragen.</div>",
   $q->hr;
 } elsif ( $hirachie eq "wa") {
  print
   $q->b( "W�chter" ),
   " (W�) eingetragen.</div>",
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
  "s ($nrow St�ck)";
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
  $q->b( "W�" ),
  "s ($nrwa St�ck)";
 foreach(@was) {
   print
   $q->font( { -color=>"FFA500" },
   	" ".$_);
 }
print <<END;
<hr>
 Die jeweiligen F�higkeiten der Oberw�chter (OW) und der W�chter (W�) sind unter
 "<a href="htdocs.pl?var=help_html">Chat-Befehle</a>"
 dokumentiert.
</div>
</body>
</html>
END
}