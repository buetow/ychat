# yChat - Copyright by Paul C. Bütow
########################### Dieser Teil bestimmt die Standart-Variabeln.
##STANDART-CONFIGURATION:## (CSS, Farben, Version etc.)
###########################
$limit = 50; 			# Benutzerlimit
$datum = "27.12.01";    	# Datum der letzten Änderung (ändern erwünscht)
$version = "0.7.0.2"; 	     	# Bitte Hauptversionsnummer nicht ändern
$title = "yChat [$version]";    # Erscheint in der Browser-Leiste oben
$standardroom = "Cyberbar";	# Standardchatraum
$loginplace = "Lounge";		# Bezeichnung der Startseite nach dem Login
$localhost = "212.202.190.233";	# Host oder IP# des Chat-Servers
$localport = 2000;		# Port über den fer Chat-Server läuft
$gfxpath = "http://$localhost/yChat"; # URL der Binärdaten (Grafiken etc.)
$bgcolor = "#3E535B";		# Hintergrundfarbe der HTML-Seiten
$chatbg = "#000000";		# Hintergrundfarbe des Chat-Frames

# PS: Um einen Chat-Administrator anzulegen, dann die Datei data/hierachie
# anlegen, in eine Zeile muß folgendes eingetragen werden: "Nickname<;7"
# (Natürlich ohne Anführungsstriche) Nickname ist somit Chat-Administrator 
# und kann Oberwächter und Wächter bestimmen!

sub style {
print  <<ENDCSS;
<style type="text/css">
 body { background-color: $bgcolor }
 body.blank { background-color: $chatbg }
 body.online { background-color: $chatbg }
 div { font-family: arial, geneva, verdana, helvetiva;  font-size: 12px; color: #ffffff }
 div.b { font-weight: bold; color: #ffa500 }
 a { color: #ffffef; }
 a:hover { color: #ffffff; }
 p {  font-family:verdana, arial, geneva, helvetica, sans-serif;  color:#FFFFFF;  font-size:12px; }
</style>
<style type="text/css" media="all">
 a { text-decoration: none; }
 a:hover { text-decoration:underline; }
 input { border:2px solid #000000; font-size:12px; color:#000000; background-color: #ffffff; height:23px; padding:2px;}
 select { border:2px solid #000000; font-family:arial, verdana, helvetica; font-size:11px; color:#000000; height:21px; padding:2px;}
</style>
ENDCSS
}
1;
