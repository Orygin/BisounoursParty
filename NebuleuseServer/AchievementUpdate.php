<?php
include("head.php");

if($g_config["Nebuleuse"] != 1)
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logFatal("Couldn't init Nebuleuse");
	exit();
}
if(!isset($_POST["PlayerID"]) || !isset($_POST["SessionId"]))
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logErro("No player id or session id transmitted");
	exit();
}
if(!isset($_POST["AchievementId"]) || !isset($_POST["AchievementProgress"]))
{
	echo NebuleuseError::NEBULEUSE_ERROR;
	$log->logError("No Achievement id or progress transmitted");
	exit();
}


$SteamID = $_POST["PlayerID"];
$SessionId = $_POST["SessionId"];
$AchievementId = $_POST["AchievementId"];
$AchievementProgress = $_POST["AchievementProgress"];

$log->logInfo("Achievement Update for player " . $SteamID);
$log->logDebug("Achievement Id : ". $AchievementId. ", Achievement Progress : ".$AchievementProgress);

$p = getPlayer($SteamID, false);

$sesid = getPlayerSessionID($p);

if($sesid != $SessionId)
{
	echo NebuleuseError::NEBULEUSE_ERROR_DISCONNECTED;
	$log->logError("AchievementUpdate : Player ". $SteamID ." has wrong session ID, had ". $SessionId . " expected " . $sesid );
	exit();
}


updateAchievementProgress($p, $AchievementId, $AchievementProgress);
echo NebuleuseError::NEBULEUSE_ERROR_NONE;
?>