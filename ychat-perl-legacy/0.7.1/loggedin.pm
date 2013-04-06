sub loggedin { # Startseite nach dem Login
 &start_html($loginplace,'blank');

 print <<END;
<div>
 <font color='$col'>$alias</font>, schön daß Du hier bist :-)
</div>
<hr>
<div>
<a href="chat?request=stream&alias=$alias&room=$standardroom&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder">Hier</a>
kannst du den Standardraum $standardroom betreten. <br><br>
END
# <form action="chat">
# <div>
#  Raum der betreten werden soll:<br>
#  <input name="room" value="Cyberbar">
#  <input type="hidden" name="request" value="stream">
#  <input type="hidden" name="alias" value="$alias">
#  <input type="hidden" name="col" value="$col">
#  <input type="hidden" name="secol" value="$secol">
#  <input type="hidden" name="tmpid" value="$tmpid">
#  <input type="hidden" name="maxVerlauf" value="$maxVerlauf">
#  <input type="hidden" name="verlaufOrder" value="$verlaufOrder">
#  <input type="submit" value="Betreten">
# </div>
# </form>
# <br>
#<div>
# Folgende Personen sind anwesend:
#</div>
#<br>
#<div>
#END

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

   my $nr = $#roomdir-1;

   if ($door || $_ eq $loginplace) {
    print $q->b( $_ )," $door ($nr) ";

   } else {
    print
     $q->b( $q->a( { -href=>"chat?request=stream&alias=$alias&room=$_&col=$col&secol=$secol&tmp=login&tmpid=$tmpid&maxVerlauf=$maxVerlauf&verlaufOrder=$verlaufOrder" }, "$_") ), " ($nr)";
   }

   print " Raumthema: $rstat[1]" if ($rstat[1] ne "\n");
   print $q->br;

   foreach(@roomdir) {
    if ($_ ne ".." && $_ ne ".") {
     print $q->a( { -href=>"chat?request=main2&var=alias_html&alias=$alias&tmp=$_&tmpid=$tmpid" }, "$_"), " ";
    }
   }

   print $q->br, $q->br;
  }
 }

 print
  $q->br, $q->br,
  $q->b( $q->font( { -color=>"ffa500" }, "Aktuelles:" ) ),
  $q->br,
  "Wie ihr seht, ist der neue yChat da. Allerdings ist es gut Möglich, daß diese Version einige Bugs beinhaltet! ",
  "Solltest du welche entdecken, dann schick doch bitte eine Mail an <a href='mailto:snooper\@ychat.de'>snooper\@ychat.de</a>.",
  "<br><br>Leider konnten noch nicht alle geplanten Features eingebaut werden. Dazu fehlte leider die Zeit.",
  " Denn mit steigendem Sourcecode leidet leider auch das schnelle Anpassungsvermögen neuer Ideen.<br><br>",
  "Aber egal, hauptsache der Chat läuft wieder :-) Mfg, Snooper ...</div>",
  $q->end_html;
}

1;
