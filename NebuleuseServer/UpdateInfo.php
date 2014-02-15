<?php
require("head.php");

echo $g_config["updaterVersion"];
echo " ";
echo $g_config["gameVersion"];
echo "\r\n";
$Sth = $g_db->prepare("SELECT Address,Name FROM ".$g_config["db_prefix"]."mirrors");

$Sth->execute();

$Sth->setFetchMode(PDO::FETCH_ASSOC);

while($row = $Sth->fetch()) {
	echo $row["Address"];
	echo " ";
	echo $row["Name"];
	echo "\r\n";
}
?>