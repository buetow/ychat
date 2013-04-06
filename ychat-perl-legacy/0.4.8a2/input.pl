#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print
 $q->header();

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
#&secure_checkid($alias);
$room = $q->param("room");
$col = $q->param("col");
$secol = $q->param("secol");
$tmp = $q->param("tmp");
$whisper = $q->param("whisper");
$maxVerlauf = $q->param("maxVerlauf");
$verlaufOrder = $q->param("verlaufOrder");

if ($maxVerlauf eq undef) { $maxVerlauf = "10";}
if ($verlaufOrder eq undef) { $verlaufOrder = "1";}

if ($tmp eq "login") {
 &zeit;
 &post($room,"<i><font color=ffffff>($hours:$min:$sec) $alias hat den Chat betreten ...</font></i>");
}

print <<END;
<html>
<head>
<script language="JavaScript"><!--

// Einstellungsvariablen
maxVerlauf = $maxVerlauf;  // Anzahl der Einträge des Verlaufs
maxPreview = 20;  // Zeichenlänge der Verlaufsvorschau
verlaufOrder = $verlaufOrder;  // Neue Nachrichten von: 0=oben, 1=unten
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
  document.inf.submit();  // Formular absenden
  document.inf.msg.focus();  // Fukus in das Eingabeform
  document.inf.msg.select();  // Text im Eigabeform markieren
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
          open ("mail.pl?alias=$alias&tmpid=$tmpid&msg=/away&col=$col&room=$room&tmp=autoaway","reload");  // 'Away' setzen
        userAway = true;
      }
    }
    else
    {
      if (autoQuitTimeout < userAwayTime)  // Falls Zeit abgelaufen
      {
        open ("mail.pl?alias=$alias&tmpid=$tmpid&msg=/q&room=$room&tmp=autologout","reload");  // Chat beenden
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
$style
</head>
<body class="blank" onload="initFrame();">
 <div align="left">
  <table border="0" cellpadding="0" cellspacing="0">
   <tr>
    <td>
     <form name="inf" target="reload" action="main.pl" onsubmit="return delout();">
      <input type="hidden" name="alias" value="$alias">
      <input type="hidden" name="room" value="$room">
      <input type="hidden" name="col" value="$col">
      <input type="hidden" name="secol" value="$secol">
      <input type="hidden" name="tmpid" value="$tmpid">
      <input type="hidden" name="whisper" value="$whisper">
      <input type="hidden" name="tmp" value="$tmp">
      <input size="50" maxlength="300" name="msg" onBlur="msgBlur();" onFocus="msgFocus();">
     </form>
    </td>
    <td><form name="verlaufform">
      <script language="JavaScript"><!--
      document.write("<select size='1' name='verlauf' size='40' onChange='CheckAuswahl();'>");
      document.write("<option value>- Nachrichten-Verlauf -</option>");
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
