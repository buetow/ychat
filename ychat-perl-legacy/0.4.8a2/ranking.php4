<!doctype html public "-//W3C//DTD HTML 4.0 //EN"> 
<html>
<head>
       <title>Top 20 Ranking</title>
       <style rel="stylesheet" type="text/css">
              body { background-color: #005146 }
              body.blank { background-color: #000000 }
              body.online { background-color: #000000 }
              div { font-family: arial, verdana, helvetiva; font-size: 9pt; color: #ffffff }
              div.b { font-weight: bold; color: #ffa500 }
              a { font-family: arial, verdana, helvetiva; font-size: 8pt; color: #FF0000 }
              a:hover { color: #000000; text-decoration: none }
       </style>
</head>
<body>
<?

$pfad = "data/user/counts";
$handle = opendir($pfad);
$counter = 0;
$datei=readdir($handle);
$datei=readdir($handle);
while($datei=readdir($handle))
{

  $ganzername = $pfad . "/" . $datei;
  $groesse = filesize($ganzername);
  $dateihandle = fopen($ganzername,"r");
  $onlinezeit = fgets($dateihandle, $groesse+1);
  $counter++;
  $user = substr($datei, 0, strlen($datei) - 7);
  $array[$user] = (int) ($onlinezeit / 60);

}
echo "<div>Es wurden " . sizeof($array) . " Eintr&auml;ge gefunden und ausgewertertet.<p> Top-40 Ranking:<br></div><HR>";
arsort($array);


echo "<table border=0 bgcolor=#000000 cellspacing=1 cellpadding=2>";

echo "<tr><td bgcolor=#DD0000 align=center>";
echo "<div> &nbsp;Rank&nbsp; </div>";
echo "<td bgcolor=#DD0000 align=center>";
echo "<div> &nbsp;Chatter&nbsp; </div>";
echo "<td bgcolor=#DD0000 align=center>";
echo "<div> &nbsp;Onlinezeit&nbsp;<br>&nbsp;[DD:HH:MM]&nbsp; </div>";
echo "</tr>";
$mykeys = array_keys($array);
$counter = 0;
$platz=0;
foreach ($mykeys as $keyname) {
        $counter++;
	    if ($counter <= 40) {
		$namedesprofils=$keyname;
		$min     = (int) ($array[$keyname] % 60);
		$tmphrs  = (int) ($array[$keyname] / 60);
		$hrs     = (int) ($tmphrs % 24);
		$tmpdays = (int) ($tmphrs / 24);
		$days    = (int) ($tmphrs / 24);

	    $onlinezeit = sprintf("%02d:%02d:%02d", $days, $hrs, $min);
        echo "<tr><td bgcolor=#EEEEEE align=center>";
        echo "<font face=\"arial,verdana,helvetiva\" size=1 color=#000000>$counter</font>";
        echo "<td bgcolor=#EEEEEE align=center>";
		echo "<a href=\"main2.pl?var=alias_html&tmp=$namedesprofils&alias=$alias&tmpid=$tmpid\" target=\"_self\">";
        echo "<font face=\"arial,verdana,helvetiva\" size=1 color=#FF0000>";
	    echo "$keyname";
		echo "</font>";
		echo "</a>";
        echo "<td bgcolor=#999999 align=center>";
        echo "<font face=\"arial,verdana,helvetiva\" size=1 color=#FFFFFF>$onlinezeit</font>";
        echo "</tr>";
		}
        if ($keyname == $alias) { $platz=$counter; }
}
echo "</table> <HR>";
echo "<div> $alias, Du befindest Dich momentan auf Platz Nr.: $platz</div>";
?>
</body>
</html>
