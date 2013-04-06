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
} elsif ($var eq "logo_html") {
 &start_html("Logo");
 print "<layer top=-3 left=0><div style=\"position:absolute; top:5; left:10\">",
 	"<img src=\"$gfxpath/y_.gif\"></div></layer>";	
 	$q->end_html;
} elsif ($var eq "menu_html") {
 &menu_html;
} elsif ($var eq "menu2_html") {
 &menu2_html;
} elsif ($var eq "werda_html") {
 &werda_html;
} elsif ($var eq "logs_html") {
 &secure_checkid($alias);
 &logs_html;
} elsif ($var eq "view_logs_html") {
 &secure_checkid($alias);
 &hirachie($alias);
 if ($hirachie eq "ow" || $hirachie eq "wa") {
  &printfile($tmp,"blank"	,"log");
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
  &start_html("Quit","blank","<script language=\"JavaScript\">open(\"index.pl?tmp=quit\",\"_top\");</script>");
  print $q->end_html;
  sleep 2;
  &remove_online($alias,$room);
  &secure_removeid($alias);
  exit;
}
ENDJS
 &start_html("Menü","blank");
print <<END;
<script language="JavaScript">
function fenster(fenster,target) {
 open(fenster,target,"width=500,height=500,screenX=100,screenY=100,scrollbars");
}
</script>
<font size=2 face=verdana>
   <a href='javascript:fenster("htdocs.pl?var=werda_html&alias=$alias&room=$room&col=$col&secol=$secol&&tmp=$tmp&tmpid=$tmpid","weristda")'>Wer ist da?</a>
   <a href='javascript:fenster("main2.pl?var=edit_profile_html&alias=$alias&tmpid=$tmpid","ProfilAendern")'>Profil ändern</a>
   <a href='javascript:fenster("main2.pl?var=edit_options_html&alias=$alias&tmpid=$tmpid","Optionen")'>Chatoptionen</a>
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

sub menu2_html {
$JavaScript = <<ENDJS;
<script language="JavaScript">
 Normal1 = new Image();
 Normal1.src = "$gfxpath/_updates.gif";
 Highlight1 = new Image();
 Highlight1.src = "$gfxpath/_updates_.gif";
 Normal2 = new Image();
 Normal2.src = "$gfxpath/_credits.gif";
 Highlight2 = new Image();
 Highlight2.src = "$gfxpath/_credits_.gif";
 Normal3 = new Image();
 Normal3.src = "$gfxpath/_readme.gif";
 Highlight3 = new Image();
 Highlight3.src = "$gfxpath/_readme_.gif";
 Normal4 = new Image();
 Normal4.src = "$gfxpath/_todo.gif";
 Highlight4 = new Image();
 Highlight4.src = "$gfxpath/_todo_.gif";

function Bildwechsel(Bildnr,Bildobjekt) {
 window.document.images[Bildnr].src = Bildobjekt.src;
}
</script>
ENDJS
 &start_html("Menü2","blank");
print <<END;
  <a onMouseOver="Bildwechsel(0,Highlight1)" onMouseOut="Bildwechsel(0,Normal1)" href="http://www.updates.yChat.de" target="Updates"><img src="$gfxpath/_updates.gif" border=0></a>
  <a onMouseOver="Bildwechsel(1,Highlight2)" onMouseOut="Bildwechsel(1,Normal2)"  href="http://www.credits.yChat.de" target="Updates"><img src="$gfxpath/_credits.gif" border=0></a>
  <a onMouseOver="Bildwechsel(2,Highlight3)" onMouseOut="Bildwechsel(2,Normal3)"  href="http://www.credits.yChat.de" target="Updates"><img src="$gfxpath/_readme.gif" border=0></a>
  <a onMouseOver="Bildwechsel(3,Highlight4)" onMouseOut="Bildwechsel(3,Normal4)"  href="http://www.credits.yChat.de" target="Updates"><img src="$gfxpath/_todo.gif" border=0></a>
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