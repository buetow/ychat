#!/usr/bin/perl -w

# yChat - Copyright by Paul C. Bütow

use CGI;
$q = new CGI;
require config;
print
 $q->header();

$alias = $q->param("alias");
$tmpid = $q->param("tmpid");
&secure_checkid($alias);
$room = $q->param("room");
$col = $q->param("col");
$secol = $q->param("secol");
$tmp = $q->param("tmp");
$whisper = $q->param('whisper');

if ($tmp eq "login") {
 &zeit;
 &post($room,"<i><font color=ffffff>($hours:$min:$sec)</font> <font color=ffa500>$alias hat den Chat betreten ...</font></i>");
}

print <<END;
<html>
 <head>
  <script language="JavaScript">

maxVerlauf = 10;  // Anzahl der Einträge des Verlaufs
maxPreview = 20;  // Zeichenlänge der Verlaufsvorschau
fullVerlauf = new Array( maxVerlauf );

function delout()
{
  addVerlauf( document.inf.msg.value );
    document.inf.submit();  // Formular absenden
    document.inf.msg.focus();  // Fukus in das Eingabeform
    document.inf.msg.select();  // Text im Eigabeform markieren
  return false;
}

function CheckAuswahl()
{
  id = document.verlaufform.verlauf.selectedIndex;

  if( fullVerlauf[ id-1 ] == null )  // Falls das Feld leer ist
    return false;  // Aufruf ignorieren

  document.inf.msg.value = fullVerlauf[ id-1 ];
  document.inf.msg.focus();

  return true;
}

function addVerlauf( msg )
{
  if( msg == "" )  // Falls msg leer
    return false;  // Aufruf ignorieren

  for( i=0; i<(maxVerlauf-1); i++ )  // Alle Einträge einen aufrutschen
  {
    document.verlaufform.verlauf.options[i+1].text = document.verlaufform.verlauf.options[i+2].text;
    fullVerlauf[i] = fullVerlauf[i+1];
  }
  if( msg.length > maxPreview )
    pre = msg.slice( 0, maxPreview-3 ) + "...";
  else
    pre = msg;

  document.verlaufform.verlauf.options[ maxVerlauf ].text = pre;  // Neue Vorschau anfügen
  fullVerlauf[ maxVerlauf-1 ] = msg;  // Neuen Eintrag anfügen

//  document.verlaufform.verlauf.selectedIndex = maxVerlauf;  // Fokus auf den letzten Eintrag
  document.verlaufform.verlauf.selectedIndex = 0;  // Fokus auf den ersten Eintrag (Überschrift)
}

  </script>

$style
<style type="text/css" media="all">
input {
 border:2px solid #000000;
 font-size:12px;
 color:#000000;
 height:23px;
 padding:2px;
}
select {
 border:2px solid #000000;
 font-family:arial, verdana, helvetica;
 font-size:11px;
 color:#000000;
 height:21px;
 padding:2px;
}
</style>
 </head>
<body class="blank" onload="document.inf.msg.focus();">
 <div align="left">
  <table border="0" cellpadding="0" cellspacing="0">
   <tr>
    <td>
     <form name="inf" target="reload" action="main.pl" onsubmit="return delout()">
      <input type="hidden" name="var" value="post_msg">
      <input type="hidden" name="alias" value="$alias">
      <input type="hidden" name="room" value="$room">
      <input type="hidden" name="col" value="$col">
      <input type="hidden" name="secol" value="$secol">
      <input type="hidden" name="tmpid" value="$tmpid">
      <input type="hidden" name="whisper" value="$whisper">
      <input type="hidden" name="tmp" value="$tmp">
      <input size="50" name="msg">
     </form>
    </td>
    <td>
     <form name="verlaufform">
      <select size="1" name="verlauf" size="40" onChange="CheckAuswahl()">
       <option value>- Nachrichten-Verlauf -</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
       <option value>(leer)</option>
      </select>
     </form>
    </td>
   </tr>
  </table>
 </div>
</body>
</html>
END
