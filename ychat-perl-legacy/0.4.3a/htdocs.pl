#!/usr/bin/perl -w

# yChat - Copyright by Paul C. B�tow

use CGI;		# CGI-Modul laden
$q = new CGI;

require config;		# config.pm in Dokument laden

print
 $q->header();		# HTTP-Header erzeugen


#####################################
##WELCHE SUB MU� AUSGEF�HRT WERDEN?##
#####################################
$var = $q->param("var");
if ($var eq "help_html") {
 &printfile("help.sno","Alle Chatbefehle");
} elsif ($var eq "logo_html") {
 &start_html("Logo");
 print "<layer top=-3 left=0><div style=\"position:absolute; top:5; left:10\">",
 	"<img src=\"$gfxpath/y_.gif\"></div></layer>";	
 	$q->end_html;
} elsif ($var eq "menu_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 $room = $q->param("room");
 $col = $q->param("col");
 $secol = $q->param("secol");
 $tmp = $q->param("tmp");
 &menu_html;
} elsif ($var eq "werda_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &werda_html;
} elsif ($var eq "logs_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 &logs_html;
} elsif ($var eq "view_logs_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 $tmp = $q->param("tmp");
 &secure_checkid($alias);
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  &printfile($tmp,"blank","log");
 }
} elsif ($var eq "superuser_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 &superuser_html;
} else {
 print
  $q->start_html(-bgcolor=>"000000"),
  $q->end_html;
}

sub menu_html {
if ($tmp eq "quit") {
  &start_html("Quit","blank","<script language=\"JavaScript\">open(\"index.pl?tmp=quit\",\"_top\");</script>");
  print $q->end_html;
  sleep 2;
  &remove_online($alias,$room);
  &secure_removeid($alias);
  exit;
}
ENDJS
 &start_html("Men�","blank");
print <<END;
<script language="JavaScript">
function fenster(fenster,target) {
 open(fenster,target,"width=500,height=500,screenX=100,screenY=100,scrollbars");
}
</script>
<font size=2 face=verdana>
   <a href='javascript:fenster("htdocs.pl?var=werda_html&alias=$alias&room=$room&col=$col&secol=$secol&&tmp=$tmp&tmpid=$tmpid","weristda")'>Wer ist da?</a>
   <a href='javascript:fenster("main2.pl?var=edit_profile_html&alias=$alias&tmpid=$tmpid&room=$room","ProfilAendern")'>Profil �ndern</a>
   <a href='javascript:fenster("main2.pl?var=edit_options_html&alias=$alias&tmpid=$tmpid&room=$room","Optionen")'>Chatoptionen</a>
   <a href='javascript:fenster("main2.pl?var=list_alias_html&alias=$alias&tmpid=$tmpid","Benutzerliste")'>Benutzerliste</a>
   <a href='javascript:fenster("htdocs.pl?var=help_html&tmpid=$tmpid","Befehle")'>Chatbefehle</a><br>
   <a href='javascript:fenster("main2.pl?var=gb_html&alias=$alias&tmpid=$tmpid","Board")'>Benutzerboard</a>
   <a href='javascript:fenster("htdocs.pl?var=superuser_html&alias=$alias&tmpid=$tmpid","Superuser")'>Superuser</a>
   <a href='javascript:fenster("htdocs.pl?var=logs_html&alias=$alias&tmpid=$tmpid","Logdateien")'>Logdateien</a>
   <a href="http://www.updates.yChat.de" target="Updates">Updates</a>
   <a href="http://www.credits.yChat.de" target="Credits">Credits</a>
   <a href="htdocs.pl?alias=$alias&var=menu_html&room=$room&tmpid=$tmpid&tmp=quit">Verlassen</a>
</font>
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
  print $q->div( "Folgende Logdateien des Raumes \"Cyberbar\" k�nnen nur von W�chtern und Oberw�chtern gelesen werden (Sortiert nach A-Z, a-z):" ),
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
  print $q->div( "Logdateien k�nnen nur von Oberw�chtern (OW) und von W�chtern (W�) gelesen werden. Du bist jedoch ein ",
  		  "\"Normaler Chatter\". W�chter k�nnen alles nachlesen, was �ffentlich gechattet wurde." ),
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