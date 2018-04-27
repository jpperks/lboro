<?php

$link = mysql_connect('xxx.xxx.xxx.xx', 'xxxxxx', 'xxxxxx');

if (!$link) {
    die('Could not connect: ' . mysql_error());
}

mysql_select_db("Jpappsdb", $link);

$time = $_GET["time"];
$lat = $_GET["lat"];
$lng = $_GET["lng"];
$sats = $_GET["sats"];
$amsl = $_GET["amsl"];
$wind = $_GET["wind"];

$result = mysql_query("INSERT INTO `Jpappsdb`.`Anemometer` (`UTC`,`Latitude`,`Longitude`,`Satellites`,`AltitudeAMSL`,`Wind Speedms`) VALUES ('$time', '$lat', '$lng', '$sats', '$amsl', '$wind')", $link); 	
if($result === FALSE) { 
    die(mysql_error()); 
}

echo "Data Recorded!";
mysql_close($link);	

?>
