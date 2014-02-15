<?php
include("head.php");

if($g_config["Nebuleuse"] != 1)
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logFatal("StatsUpdate : Couldn't init Nebuleuse");
	exit();
}
if(!isset($_POST["PlayerID"]) || !isset($_POST["SessionId"]))
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logError("StatsUpdate : Player id or session id not transmitted");
	exit();
}
if(!isset($_POST["Stats"]))
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logError("StatsUpdate : Stats data not transmitted");
	exit();
}


$SteamID = $_POST["PlayerID"];
$SessionId = $_POST["SessionId"];
$Stats = $_POST["Stats"];

$p = getPlayer($SteamID, true);

$sesid = getPlayerSessionID($p);

if($sesid != $SessionId)
{
	echo NebuleuseError::NEBULEUSE_ERROR_DISCONNECTED;
	$log->logError("StatsUpdate : Player ". $SteamID ." has wrong session ID, had ". $SessionId . " expected " . $sesid );
	exit();
}

UpdateStats($p, $Stats);
$log->logInfo("StatsUpdate : Updated player " . $SteamID . " stats");
echo NebuleuseError::NEBULEUSE_ERROR_NONE;
?>