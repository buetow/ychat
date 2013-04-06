
sub htdocs {

 if ($var eq "help_html") {
  &printfile("help.sno","Alle Chatbefehle");

 } elsif ($var eq "logo_html") {
  &start_html("Logo");
  &logo_html;

 } elsif ($var eq "menu_html") {
  &menu_html;

 } elsif ($var eq "logs_html") {
  &logs_html;

 } elsif ($var eq "view_logs_html") {
  &secure_checkid($alias,$tmpid);
  &printfile("data/logs/$tmp","Logdatei","blank")
   if (5 <= &hierachie($alias));

 } elsif ($var eq "superuser_html") {
  &secure_checkid($alias,$tmpid);
  &superuser_html;

 } elsif ($var eq "chatiquette_html") {
  &printfile("chatiq.sno","Chatiquette");
  print $q->end_html;

 } else {
  print
   $q->start_html(-bgcolor=>"000000"),
   $q->end_html;
 }
}

sub menu_html {
 &start_html("Menü");
print <<END;
<div>
 Von hier aus kannst du alle Untermenüpunkte des Systems erreichen:
</div>
<hr>
END
&menu();
print "</body>\n</html>";
}

sub logo_html {
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
  <PARAM NAME=bgcolor VALUE=$bgcolor>
  <EMBED src="$gfxpath/ySymbol.swf" loop=false menu=false quality=high scale=noborder wmode=opaque bgcolor=$bgcolor  WIDTH=48 HEIGHT=48 TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></EMBED>
 </OBJECT>
 </div></layer>
END
 print $q->end_html;
}



sub logs_html {
 &start_html("Logdateien");
 &menu;
 print <<END;
<div>
 Hier befinden sich alle Logdateien
</div>
<hr>
END
 if (5 <= &hierachie($alias)) {
  print $q->div( "Folgende Logdateien des Raumes \"$standardroom\" können nur von Oberwächtern oder höher gelesen werden (Sortiert nach A-Z, a-z):" ),
        $q->br;

  opendir(LOGS,"data/logs");
   my @logs = readdir(LOGS);
  closedir(LOGS);

  my @sortedir = sort(@logs);

  foreach(@sortedir) {

   if (-f "data/logs/$_") {
    print $q->a( { -href=>"chat?request=htdocs&alias=$alias&var=view_logs_html&tmp=$_&tmpid=$tmpid",
                     -target=>"_new" },
           "$_" ),
           " ";
   }
  }
 } else {
  print $q->div( "Logdateien können nur von Oberwächtern (OW) gelesen werden. Du bist jedoch ein Wächter (WÄ) oder ein",
                    "\"Normaler Chatter\". Oberwächter können alles nachlesen, was öffentlich im Raum \"Cyberbar\" gechattet wurde." ),
  $q->hr;
 }
 print
  $q->end_html;
}

sub superuser_html { # Zeigt an, wer   Superuser ist
 start_html("Superuser");
 &menu;

 print "<div>Hallo $alias. Du bist als ";
 my $hierachie = &hierachie($alias);
 if (7 <= $hierachie) {
  print
   $q->b( "Administrator" ),
   " (AD) eingetragen.</div><br>",
   $q->br;
 } elsif (5 <= $hierachie) {
  print
   $q->b( "Oberwächter" ),
   " (OW) eingetragen.</div><br>",
   $q->br;
 } elsif (3 <= $hierachie) {
  print
   $q->b( "Wächter" ),
   " (WÄ) eingetragen.</div><br>",
   $q->br;
 } else {
  print
  $q->b( "normaler Chatter" ),
  " (NC) eingetragen.</div><br>",
  $q->br;
 }

open FILE, "data/hierachie";
 my @hierachie = <FILE>;
close FILE;

my @ows, @was, @ads;
foreach (@hierachie) {
 my ($name,$level) = split "<;";
 chomp $level;

 if ($level == 3) {
  push @was, $name;

 } elsif ($level == 5) {
  push @ows, $name;

 } elsif ($level == 7) {
  push @ads, $name;
 }


 if (3 <= $hierachie) {
  if ($level == 4) {
   push @was, "$name!";

  } elsif ($level == 6) {
   push @ows, "$name!";

  } elsif ($level == 8) {
   push @ads, "$name!" ;
  }
 }
}

 my $nads = $#ads+1;
 print "<div>", $q->b( "AD" ), "s ($nads Stück)";

 foreach(@ads) {
  print $q->font( { -color=>"ffa500" }, " ".$_);
 }

 my $nrow = $#ows+1;
 print "</div><br><br><div>", $q->b( "OW" ), "s ($nrow Stück)";

 foreach(@ows) {
  print $q->font( { -color=>"ffa500" }," ".$_);
 }

 my $nrwa = $#was+1;
 print "</div><br>", $q->br, "<div>", $q->b( "WÄ" ),"s ($nrwa Stück)";

 foreach(@was) {
   print $q->font( { -color=>"FFA500" }," ".$_);
 }

print <<END;
<hr>
 Die jeweiligen Fähigkeiten der Oberwächter (OW) und der Wächter (WÄ) sind unter
 "<a href="chat?request=htdocs&var=help_html">Chat-Befehle</a>"
 dokumentiert. Versteckte Superuser können nur von anderen Superusern auf
dieser Liste eingesehen werden!
</div>
</body>
</html>
END
}

sub menu {
print <<MENUEND;
<div>
  <a href='chat?request=main2&var=edit_profile_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Profil ändern</a>
  <a href='chat?request=main2&var=edit_options_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Optionen</a>
  <a href='chat?request=main2&var=list_alias_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Benutzer</a>
  <a href='chat?request=htdocs&var=help_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Befehle</a>
  <a href='chat?request=main2&var=gb_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Board</a>
  <a href='chat?request=htdocs&var=superuser_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Superuser</a>
  <a href='chat?request=htdocs&var=logs_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Logs</a>
  <a href='chat?request=stat&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Statistik</a>
  <a href='chat?request=htdocs&var=chatiquette_html&alias=$alias&tmp=$tmp&tmpid=$tmpid'>Chatiquette</a>
</div>
<hr>
MENUEND
}


1;
