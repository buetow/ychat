sub input {

 &rm_away($alias,$room);

 $maxVerlauf = "10"
  if ($maxVerlauf eq undef);

 $verlaufOrder = "1"
  if ($verlaufOrder eq undef);

 if ($tmp eq "login") {
  &post($room,"<i><font color=ffffff>".&timestring." <font color='$col'>$alias</font> hat den Chat im Raum <b>$room</b> betreten ...</font></i>");
 }

print <<END;
<html>
<head>
<script language="JavaScript"><!--

// Einstellungsvariablen
maxVerlauf = $maxVerlauf;  // Anzahl der Einträge des Verlaufs
maxPreview = 20;  // Zeichenlänge der Verlaufsvorschau
verlaufOrder = $verlaufOrder;  // Neue Nachrichten von: 0=oben, 1=unten
//autoAwayTimeout = 5;  // Sekunden, nach denen der User 'Away' gesetzt wird
//autoQuitTimeout = 10;  // Sekunden, nach denen der User den Chat verlässt
autoAwayTimeout = 300;  // Sekunden, nach denen der User 'Away' gesetzt wird
autoQuitTimeout = 1200;  // Sekunden, nach denen der User den Chat verlässt

// Interne Variablen
userNotFocused = false;  // Wenn User nicht schreibt
userAway = false;  // Wenn User automatisch auf 'Away' gesetzt ist
userAwayTime = 0;  // Sekunden, die User 'Away' ist
fullVerlauf = new Array( maxVerlauf );  // Verlaufspuffer

function delout()
{
  addVerlauf (document.inf.msg.value);  // Dem Verlauf eine Nachricht anhängen
  document.inf.msg.focus();  // Fukus in das Eingabeform
  document.inf.msg.select();  // Text im Eigabeform markieren
  document.inf.submit();  // Formular absenden
  return false;  // Abbrechen, damit das Frame nicht neu geladen wird
}

function CheckAuswahl()
{
  id=document.verlaufform.verlauf.selectedIndex;   // ID des ausgewählten Feldes

  if (fullVerlauf[id-1] != null)  // Nur, wenn das Feld nicht leer ist
  {
    document.inf.msg.value = fullVerlauf[id-1];  // Eintrag ins Textfeld
  }

  document.verlaufform.verlauf.selectedIndex = 0;  // Ersten Eintrag (Überschrift) auswählen
  document.inf.msg.focus();  // Eingabefeld fokusieren

  return true;
}

function addVerlauf (msg)
{
  if (msg == "")  // Falls msg leer
    return false;  // Aufruf ignorieren

  if (verlaufOrder)  // neue Nachrichten von unten
  {
    for (i=0; i<(maxVerlauf-1); i++)  // Alle Einträge einen aufrutschen
    {
      document.verlaufform.verlauf.options[i+1].text = document.verlaufform.verlauf.options[i+2].text;
      fullVerlauf[i] = fullVerlauf[i+1];
    }
  }
  else  // neue Nachrichten von oben
  {
    for (i=(maxVerlauf-2); i>=0; i--)  // Alle Einträge einen aufrutschen
    {
      document.verlaufform.verlauf.options[i+2].text = document.verlaufform.verlauf.options[i+1].text;
      fullVerlauf[i+1] = fullVerlauf[i];
    }
  }

  if (msg.length > maxPreview)  // Vorschau zuschneiden
    pre = msg.slice(0, maxPreview-3) + "...";
  else
    pre = msg;

  if (verlaufOrder)  // pos = Position des neuen Eintrags
    pos = maxVerlauf-1;
  else
    pos = 0

  document.verlaufform.verlauf.options[pos+1].text = pre;  // Neue Vorschau anfügen
  fullVerlauf[pos] = msg;  // Neuen Eintrag an den Verlauf anfügen

//  document.verlaufform.verlauf.selectedIndex = maxVerlauf;  // Fokus auf den letzten Eintrag
  document.verlaufform.verlauf.selectedIndex = 0;  // Fokus auf den ersten Eintrag (Überschrift)
}

function msgBlur()
{
  userNotFocused = true;
  userAwayTime = 0;
  userAway = false;
}

function msgFocus()
{
  userNotFocused = false;
  if (userAway)  // Wird ausgeführt, falls User 'Away' war
  {
    ;  // nop
  }
  userAwayTime = 0;
  userAway = false;
}

function checkInterval()
{
//  status = "userNotFocused=" + userNotFocused + "; userAway=" + userAway + "; userAwayTime=" + userAwayTime + "";  // Debug
  if (userNotFocused)
  {
    userAwayTime += 5;  // Addiere 5, da nur alle 5Sek ausgeführt
    if (!userAway)
    {
      if (autoAwayTimeout < userAwayTime)  // Falls Zeit abgelaufen
      {
        if (document.inf.tmp.value != "away")
          open ("chat?request=main1&alias=$alias&tmpid=$tmpid&msg=%2faway&tmp=autoaway","reload");  // 'Away' setzen
        userAway = true;
      }
    }
    else
    {
      if (autoQuitTimeout < userAwayTime)  // Falls Zeit abgelaufen
      {
        open ("chat?request=main1&alias=$alias&tmpid=$tmpid&msg=%2fexit&tmp=autologout","reload");  // Chat beenden
      }
    }
  }

  window.setTimeout("checkInterval()",5000);  // In 5 Sekunden wieder aufrufen
}

function initFrame()
{
  window.setTimeout("checkInterval()",5000);  // In 5 Sekunden aufrufen
  document.inf.msg.focus();  // Eingabefeld fokusieren
}
//-->
</script>
END

&style;

print <<END;
</head>
<body class="blank" onload="initFrame();">
 <div align="left">
  <table border="0" cellpadding="0" cellspacing="0">
   <tr>
    <td>
     <form name="inf" target="reload" action="chat" onsubmit="return delout();">
      <input type="hidden" name="request" value="main1">
      <input type="hidden" name="alias" value="$alias">
      <input type="hidden" name="room" value="$room">
      <input type="hidden" name="col" value="$col">
      <input type="hidden" name="secol" value="$secol">
      <input type="hidden" name="tmpid" value="$tmpid">
      <input type="hidden" name="whisper" value="$whisper">
      <input type="hidden" name="tmp" value="$tmp">
      <input size="40" maxlength="300" name="msg" onBlur="msgBlur();" onFocus="msgFocus();">
     </form>
    </td>
    <td><form name="verlaufform">
      <script language="JavaScript"><!--
      document.write("<select size='1' name='verlauf' size='40' onChange='CheckAuswahl();'>");
      document.write("<option>- Nachrichten-Verlauf -</option>");
      for(i=0; i<maxVerlauf; i++)  // Leere Verlaufsfelder generieren
        document.write("<option value>(leer)</option>");
      document.write("</select>");
     //-->
      </script>
     </form>
    </td>
   </tr>
  </table>
 </div>
</body>
</html>
END

$client->$shutdown(2);
exit 0;
}

1;
