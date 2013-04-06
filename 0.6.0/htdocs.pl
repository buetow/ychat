#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print $q->header;

$var = $q->param("var");

if ($var eq "help_html") {
 &printfile("help.sno","Alle Chatbefehle");
} elsif ($var eq "logo_html") {
 &start_html("Logo");
print<<END;
<layer top=-3 left=0><div style="position:absolute; top:5; left:10">
<OBJECT classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
 codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=5,0,0,0"
 WIDTH=48 HEIGHT=48>
 <PARAM NAME=movie VALUE="$gfxpath/ySymbol.swf">
 <PARAM NAME=loop VALUE=false>
 <PARAM NAME=menu VALUE=false>
 <PARAM NAME=quality VALUE=high>
 <PARAM NAME=scale VALUE=noborder>
 <PARAM NAME=wmode VALUE=opaque>
 <PARAM NAME=bgcolor VALUE=#005146>
 <EMBED src="$gfxpath/ySymbol.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=#005146
 WIDTH=48 HEIGHT=48 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>
</OBJECT>
</div></layer>
END
print $q->end_html;
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
 if (&hierachie($alias) eq "ow") {
  &printfile($tmp,"Logdatei","blank");
 }
} elsif ($var eq "superuser_html") {
 $alias = $q->param("alias");
 $tmpid = $q->param("tmpid");
 &secure_checkid($alias);
 &superuser_html;
} elsif ($var eq "chatiquette_html") {
 &printfile("chatiq.sno","Chatiquette");
} else {
 print
  $q->start_html(-bgcolor=>"000000"),
  $q->end_html;
}

sub menu_html {
if ($tmp eq "quit") {
  &start_html("Quit","blank","<script language=\"JavaScript\">open(\"index.pl?tmp=quit\",\"_top\");</script>");
  print $q->end_html;
  &zeit;
  &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffffff>$alias verläßt den Chat ... </font></i>");
  sleep 2;
  &rm_alias($alias,$room);
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
<font size=2 face=verdana><p>
   <a href='javascript:fenster("htdocs.pl?var=werda_html&alias=$alias&room=$room&col=$col&secol=$secol&tmp=$tmp&tmpid=$tmpid","weristda")'>Wer ist da?</a>
   <a href='javascript:fenster("main2.pl?var=edit_profile_html&alias=$alias&tmpid=$tmpid&room=$room","ProfilAendern")'>Profil ändern</a>
   <a href='javascript:fenster("main2.pl?var=edit_options_html&alias=$alias&tmpid=$tmpid&room=$room","Optionen")'>Optionen</a>
   <a href='javascript:fenster("main2.pl?var=list_alias_html&alias=$alias&tmpid=$tmpid","Benutzerliste")'>Benutzer</a>
   <a href='javascript:fenster("htdocs.pl?var=help_html&tmpid=$tmpid","Befehle")'>Befehle</a>
   <a href='javascript:fenster("main2.pl?var=gb_html&alias=$alias&tmpid=$tmpid","Board")'>Board</a><br>
   <a href='javascript:fenster("htdocs.pl?var=superuser_html&alias=$alias&tmpid=$tmpid","Superuser")'>Superuser</a>
   <a href='javascript:fenster("htdocs.pl?var=logs_html&alias=$alias&tmpid=$tmpid","Logdateien")'>Logs</a>
   <a href='javascript:fenster("stat.pl?&alias=$alias&tmpid=$tmpid","Statistik")'>Statistik</a>
   <a href='javascript:fenster("htdocs.pl?var=chatiquette_html","Chatiquette")'>Chatiquette</a>
   <a href="http://www.home.yChat.de" target="Home">Home</a>
   <a href="http://www.credits.yChat.de" target="Credits">Credits</a>
   <a href="htdocs.pl?alias=$alias&var=menu_html&room=$room&tmpid=$tmpid&tmp=quit">Verlassen</a>
</p></font>
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
 opendir(ROOMDIRS,"data/online/rooms");
  local @roomdirs = readdir(ROOMDIRS);
 close(ROOMDIRS);
 foreach(@roomdirs) {
  if (-e "data/online/rooms/$_" && $_ ne ".." && $_ ne ".") {
   my $room = $_;
   open(RSTAT,"<data/online/rstat/$room");
    my @rstat = <RSTAT>;
   close RSTAT;
   my $door = "[<font color=red>Abgesperrt!</font>]" if ($rstat[0] eq "1\n");
   opendir(ROOMDIR,"data/online/rooms/$room");
    my @roomdir = readdir(ROOMDIR);
   close(ROOMDIR);
   local $nr = $#roomdir-1;
   print $q->b( $_ ),
         " $door ($nr) Raumthema: $rstat[1]",
         $q->br;
   foreach(@roomdir) {
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
 if (&hierachie($alias) eq "ow") {
  print $q->div( "Folgende Logdateien des Raumes \"Cyberbar\" können nur von Oberwächtern gelesen werden (Sortiert nach A-Z, a-z):" ),
        $q->br;
  opendir(LOGS,"data/logs");
   local @logs = readdir(LOGS);
  closedir(LOGS);
  local @sortedir = sort(@logs);
  foreach(@sortedir) {
   if (-f "data/logs/$_") {
    print $q->a( { -href=>"htdocs.pl?alias=$alias&var=view_logs_html&tmp=data/logs/$_&tmpid=$tmpid",
                     -target=>"_new" },
           "$_" ),
           " ";
   }
  }
 } else {
  print $q->div( "Logdateien können nur von Oberwächtern (OW) gelesen werden. Du bist jedoch ein Wächter (WÄ) oder ein",
                    "\"Normaler Chatter\". Oberwächter können alles nachlesen, was öffentlich im Raum \"Cyberbar\" gechattet wurde." ),
  $q->hr;
  &printfile("data/htdocs/logs_html");
 }
 print
  $q->end_html;
}

sub superuser_html { # Zeigt an, wer   Superuser ist
 start_html("Superuser");
 print "<div>Hallo $alias. Du bist als ";
 if (hierachie($alias) eq "ow") {
  print
   $q->b( "Oberwächter" ),
   " (OW) eingetragen.</div>",
   $q->hr;
 } elsif (hierachie($alias) eq "wa") {
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

 open WA, "<data/wa";
  my @was = <WA>;
 close WA;
 open OW, "<data/ow";
  my @ows = <OW>;
 close OW;

 my $nrow = $#ows+1;
 print
  "<div>",
  $q->b( "OW" ),
  "s ($nrow Stück)";
 foreach(@ows) {
  print
   $q->font( { -color=>"ffa500" },
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
 "<a href="htdocs.pl?var=help_html">Chat-Befehle</a>"
 dokumentiert.
</div>
</body>
</html>
END
}