<?php //db class

if(!$g_config["Nebuleuse"])
{
	echo 'Config file not loaded';
	exit();
}

$db = new PDO('mysql:host=' . $g_config["db_host"] . ';dbname='. $g_config["db_base"] .'', $g_config["db_user"], $g_config["db_pw"]);
$Sth = $db->prepare("SELECT name,value FROM ".$g_config["db_prefix"]."config");
$Sth->execute();
$Sth->setFetchMode(PDO::FETCH_ASSOC);
while($row = $Sth->fetch()) { //Setup config array
	$g_config[$row["name"]] = $row["value"];
}

Flight::set('db', $db);
Flight::set('config', $g_config);