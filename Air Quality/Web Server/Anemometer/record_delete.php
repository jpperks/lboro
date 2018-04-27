<?php

$link = mysql_connect('xxx.xxx.xxx.xx', 'xxxxxx', 'xxxxxx');

if (!$link) {
    die('Could not connect: ' . mysql_error());
}

mysql_select_db("Jpappsdb", $link);

$true = $_GET["true"];
$result = mysql_query( "TRUNCATE TABLE Jpappsdb.Anemometer" ); 	
if($result === FALSE) { 
    die(mysql_error()); 
}

echo "Data Deleted!";
mysql_close($link);	

?>
