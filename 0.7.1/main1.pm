sub main1 {

 if (!-e "data/online/rooms/$room/$alias" && $tmp ne "autoaway" && $tmp ne "autologout") {
   &post_prv($alias,"<font color=ffffff>Du befindest Dich nicht im Raum <font color=ffa500>$room</font> und kannst dort auch keine Nachrichten posten!</font>");
   &reload_html;

 } elsif (length($msg) > 300) {
  &post_prv($alias,"<font color=ffffff>Deine Nachricht enthält zu viele Zeichen. Maximal sind 300 Zeichen erlaubt.</font>");
  &reload_html;
 }

 if ($tmp eq "away") {
  &rm_away($alias,$room);
  &post($room,"<i><font color=ffffff>".&timestring." <font color='$col'>$alias</font> hat sich zurück gemeldet ... </font></i><script language='JavaScript'>parent.online.location.reload();</script>", $alias);
  &private_javascript($alias, $room, "parent.input.document.inf.tmp.value = 'notaway';");
 }

 $msg =~ s#<#&lt\;#g;

 if (substr($msg,0,1) ne "/") {

  unless ($tmp eq "autoaway") {
   &prove_color($alias,$_)
    foreach($col, $secol);
  }

  open MSGFILE,">>data/msgs/$room";
   unless ($msg =~ s#/me/#<i>$alias</i>#g || $msg =~ s#/m/#<i><font color=$col>$alias</font></i>#g) {
    print MSGFILE "!<;".time."<;!<;!<;<b><font color='$col'>$alias: <font color='$secol'>$msg<br></font></font></b><;$alias<;\n";

   } else {
    $msg =~ s#/me/#<i>$alias</i>#g;
    $msg =~ s#/m/#<i><font color=$col>$alias</font></i>#g;
    print MSGFILE "!<;".time."<;!<;!<;<b><font color=$secol>$msg</font></font></b><br><;$alias<;\n";
   }
  close MSGFILE;

  opendir PID,"data/online/pids/$room";
   my @pids = readdir(PID);
  closedir(PID);

  foreach(@pids) {
   kill USR1 => $_
    if (-f "data/online/pids/$room/$_");
  }

  &log("<b><font color=$col>$alias: <font color=$secol>$msg</font></font></b>")
   if ($room eq $standardroom);

  &reload_html;

 } else {
  &command($msg,$tmp);
 }
}

1;
