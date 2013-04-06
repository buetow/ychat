sub input {
    &rm_away($alias, $room);
    $maxVerlauf = '10' if $maxVerlauf eq undef;
    $verlaufOrder = '1' if $verlaufOrder eq undef;
    if ($tmp eq 'login') {
        &post($room, '<i><font color=ffffff>' . &timestring . " <font color='$col'>$alias</font> hat den Chat im Raum <b>$room</b> betreten ...</font></i>");
    }
    print qq[<html>\n<head>\n<script language="JavaScript"><!--\n\n// Einstellungsvariablen\nmaxVerlauf = $maxVerlauf;  // Anzahl der Eintr\344ge des Verlaufs\nmaxPreview = 20;  // Zeichenl\344nge der Verlaufsvorschau\nverlaufOrder = $verlaufOrder;  // Neue Nachrichten von: 0=oben, 1=unten\n//autoAwayTimeout = 5;  // Sekunden, nach denen der User 'Away' gesetzt wird\n//autoQuitTimeout = 10;  // Sekunden, nach denen der User den Chat verl\344sst\nautoAwayTimeout = 300;  // Sekunden, nach denen der User 'Away' gesetzt wird\nautoQuitTimeout = 1200;  // Sekunden, nach denen der User den Chat verl\344sst\n\n// Interne Variablen\nuserNotFocused = false;  // Wenn User nicht schreibt\nuserAway = false;  // Wenn User automatisch auf 'Away' gesetzt ist\nuserAwayTime = 0;  // Sekunden, die User 'Away' ist\nfullVerlauf = new Array( maxVerlauf );  // Verlaufspuffer\n\nfunction delout()\n{\n  addVerlauf (document.inf.msg.value);  // Dem Verlauf eine Nachricht anh\344ngen\n  document.inf.msg.focus();  // Fukus in das Eingabeform\n  document.inf.msg.select();  // Text im Eigabeform markieren\n  document.inf.submit();  // Formular absenden\n  return false;  // Abbrechen, damit das Frame nicht neu geladen wird\n}\n\nfunction CheckAuswahl()\n{\n  id=document.verlaufform.verlauf.selectedIndex;   // ID des ausgew\344hlten Feldes\n\n  if (fullVerlauf[id-1] != null)  // Nur, wenn das Feld nicht leer ist\n  {\n    document.inf.msg.value = fullVerlauf[id-1];  // Eintrag ins Textfeld\n  }\n\n  document.verlaufform.verlauf.selectedIndex = 0;  // Ersten Eintrag (\334berschrift) ausw\344hlen\n  document.inf.msg.focus();  // Eingabefeld fokusieren\n\n  return true;\n}\n\nfunction addVerlauf (msg)\n{\n  if (msg == "")  // Falls msg leer\n    return false;  // Aufruf ignorieren\n\n  if (verlaufOrder)  // neue Nachrichten von unten\n  {\n    for (i=0; i<(maxVerlauf-1); i++)  // Alle Eintr\344ge einen aufrutschen\n    {\n      document.verlaufform.verlauf.options[i+1].text = document.verlaufform.verlauf.options[i+2].text;\n      fullVerlauf[i] = fullVerlauf[i+1];\n    }\n  }\n  else  // neue Nachrichten von oben\n  {\n    for (i=(maxVerlauf-2); i>=0; i--)  // Alle Eintr\344ge einen aufrutschen\n    {\n      document.verlaufform.verlauf.options[i+2].text = document.verlaufform.verlauf.options[i+1].text;\n      fullVerlauf[i+1] = fullVerlauf[i];\n    }\n  }\n\n  if (msg.length > maxPreview)  // Vorschau zuschneiden\n    pre = msg.slice(0, maxPreview-3) + "...";\n  else\n    pre = msg;\n\n  if (verlaufOrder)  // pos = Position des neuen Eintrags\n    pos = maxVerlauf-1;\n  else\n    pos = 0\n\n  document.verlaufform.verlauf.options[pos+1].text = pre;  // Neue Vorschau anf\374gen\n  fullVerlauf[pos] = msg;  // Neuen Eintrag an den Verlauf anf\374gen\n\n//  document.verlaufform.verlauf.selectedIndex = maxVerlauf;  // Fokus auf den letzten Eintrag\n  document.verlaufform.verlauf.selectedIndex = 0;  // Fokus auf den ersten Eintrag (\334berschrift)\n}\n\nfunction msgBlur()\n{\n  userNotFocused = true;\n  userAwayTime = 0;\n  userAway = false;\n}\n\nfunction msgFocus()\n{\n  userNotFocused = false;\n  if (userAway)  // Wird ausgef\374hrt, falls User 'Away' war\n  {\n    ;  // nop\n  }\n  userAwayTime = 0;\n  userAway = false;\n}\n\nfunction checkInterval()\n{\n//  status = "userNotFocused=" + userNotFocused + "; userAway=" + userAway + "; userAwayTime=" + userAwayTime + "";  // Debug\n  if (userNotFocused)\n  {\n    userAwayTime += 5;  // Addiere 5, da nur alle 5Sek ausgef\374hrt\n    if (!userAway)\n    {\n      if (autoAwayTimeout < userAwayTime)  // Falls Zeit abgelaufen\n      {\n        if (document.inf.tmp.value != "away")\n          open ("chat?request=postmessage&alias=$alias&tmpid=$tmpid&msg=%2faway&tmp=autoaway","reload");  // 'Away' setzen\n        userAway = true;\n      }\n    }\n    else\n    {\n      if (autoQuitTimeout < userAwayTime)  // Falls Zeit abgelaufen\n      {\n        open ("chat?request=postmessage&alias=$alias&tmpid=$tmpid&msg=%2fexit&tmp=autologout","reload");  // Chat beenden\n      }\n    }\n  }\n\n  window.setTimeout("checkInterval()",5000);  // In 5 Sekunden wieder aufrufen\n}\n\nfunction initFrame()\n{\n  window.setTimeout("checkInterval()",5000);  // In 5 Sekunden aufrufen\n  document.inf.msg.focus();  // Eingabefeld fokusieren\n}\n//-->\n</script>\n];
    &style;
    print qq[</head>\n<body class="blank" onload="initFrame();">\n <div align="left">\n  <table border="0" cellpadding="0" cellspacing="0">\n   <tr>\n    <td>\n     <form name="inf" target="reload" action="chat" onsubmit="return delout();">\n      <input type="hidden" name="request" value="postmessage">\n      <input type="hidden" name="alias" value="$alias">\n      <input type="hidden" name="room" value="$room">\n      <input type="hidden" name="col" value="$col">\n      <input type="hidden" name="secol" value="$secol">\n      <input type="hidden" name="tmpid" value="$tmpid">\n      <input type="hidden" name="whisper" value="$whisper">\n      <input type="hidden" name="tmp" value="$tmp">\n      <input size="40" maxlength="300" name="msg" onBlur="msgBlur();" onFocus="msgFocus();">\n     </form>\n    </td>\n    <td><form name="verlaufform">\n      <script language="JavaScript"><!--\n      document.write("<select size='1' name='verlauf' size='40' onChange='CheckAuswahl();'>");\n      document.write("<option>- Nachrichten-Verlauf -</option>");\n      for(i=0; i<maxVerlauf; i++)  // Leere Verlaufsfelder generieren\n        document.write("<option value>(leer)</option>");\n      document.write("</select>");\n     //-->\n      </script>\n     </form>\n    </td>\n   </tr>\n  </table>\n </div>\n</body>\n</html>\n];
    $client->$shutdown(2);
    exit 0;
}
1;
