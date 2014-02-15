<?php
require("config.php");
require("functions.php");
require("KLogger.php");

$log = new KLogger('logs', $g_config["log_level"]);

$g_db = new PDO('mysql:host=' . $g_config["db_host"] . ';dbname='. $g_config["db_base"] .'', $g_config["db_user"], $g_config["db_pw"]);
$g_db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$Sth = $g_db->prepare("SELECT name,value FROM ".$g_config["db_prefix"]."config");
$Sth->execute();
$Sth->setFetchMode(PDO::FETCH_ASSOC);
while($row = $Sth->fetch()) { //Setup config array
	$g_config[$row["name"]] = $row["value"];
}

if($g_config["maintenance"] == 1){
	$output = array('maintenance' => true, "subMessage" => $g_config["maintenance_message"] );
	echo json_encode($output);
	exit();
}