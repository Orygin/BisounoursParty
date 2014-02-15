<?php
include("head.php");

if($g_config["Nebuleuse"] != 1)
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	exit();
}

$Sth = $g_db->prepare("DELETE FROM ".$g_config["db_prefix"]."sessions WHERE NOW() > Date_Add( lastAlive, INTERVAL :sessiontimeout SECOND )");
$Sth->bindParam(":sessiontimeout", $g_config["Session_timeout"]);
$Sth->execute();

$log->logInfo("Purged inactive sessions");
?>