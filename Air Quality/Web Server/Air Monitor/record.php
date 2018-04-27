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
$temp = $_GET["temp"];
$press = $_GET["press"];
$agl = $_GET["agl"];
$nh3 = $_GET["nh3"];
$co = $_GET["co"];
$no2 = $_GET["no2"];
$c3h8 = $_GET["c3h8"];
$c4h10 = $_GET["c4h10"];
$ch4 = $_GET["ch4"];
$h2 = $_GET["h2"];
$c2h5oh = $_GET["c2h5oh"];
$mq2 = $_GET["mq2"];
$mq5 = $_GET["mq5"];
$aqi = $_GET["aqi"];

$result = mysql_query("INSERT INTO `Jpappsdb`.`AirQuality` (`UTC`,`Latitude`,`Longitude`,`Satellites`,`AltitudeAMSL`,`TemperatureC`,`PressurehPa`,`AltitudeAGL`,`NH3ugm3`,`COmgm3`,`NO2mgm3`,`C3H8ppm`,`C4H10ppm`,`CH4mgm3`,`H2ugm3`,`C2H5OHppm`,`MQ2`,`MQ5`,`AQI`) VALUES ('$time', '$lat', '$lng', '$sats', '$amsl', '$temp', '$press', '$agl', '$nh3', '$co', '$no2', '$c3h8', '$c4h10', '$ch4', '$h2', '$c2h5oh', '$mq2', '$mq5', '$aqi')", $link); 	
if($result === FALSE) { 
    die(mysql_error()); 
}

echo "Data Recorded!";
mysql_close($link);	

?>
