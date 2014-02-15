<?php
include("head.php");

if($g_config["Nebuleuse"] != 1)
	exit();
if(!isset($_POST["PlayerID"]))
	exit();

$SteamID = $_POST["PlayerID"];

$p = getPlayer($SteamID);

if($p["rank"] == 0){
	$output["gameVersion"] = (int)$g_config["gameVersion"];
	$output["rank"] = (int)$p["rank"];
	$output["steamid64"] = $SteamID;
	echo json_encode($output);
	$log->logWarn("Player is banned : " . stripslashes($SteamID));
	exit();
}

$Sth = $g_db->prepare("SELECT COUNT(*) FROM ".$g_config["db_prefix"]."sessions WHERE playerid = :id");
$Sth->bindParam(":id", $p["id"]);
$Sth->execute();
$Sth->setFetchMode(PDO::FETCH_ASSOC);
$res = $Sth->fetch();

if($res["COUNT(*)"] == "1")
{
	$Del = $g_db->prepare("DELETE FROM ".$g_config["db_prefix"]."sessions WHERE playerid = :id");
	$Del->bindParam(":id", $p["id"]);
	$Del->execute();
	$log->logWarn("Player logged more than once. Deleted duplicate players sessions");
}

$SessionID = fmod($SteamID*time(), 10000000) . fmod($SteamID*time(),$SteamID%97);
$log->logDebug("Player " . $SteamID . " assigned with session id : " . $SessionID);

$Sth = $g_db->prepare("INSERT INTO ".$g_config["db_prefix"]."sessions (playerid,lastAlive,sessionId,sessionStart) VALUES (:id,NOW(),:sessionid,NOW());
							UPDATE ".$g_config["db_prefix"]."players SET LastPlayedVersion = :gameVersion WHERE id = :id;");
$Sth->bindParam(":id", $p["id"]);
$Sth->bindParam(":sessionid", $SessionID);
$Sth->bindParam(":gameVersion", $g_config["gameVersion"]);
$Sth->execute();

$output["gameVersion"] = (int)$g_config["gameVersion"];
$output["version_major"] = $g_config["version_major"];
$output["version_minor"] = $g_config["version_minor"];
$output["version_patch"] = $g_config["version_patch"];
$output["subMessage"] = $g_config["subMessage"];
$output["rank"] = (int)$p["rank"];
$output["steamid64"] = $SteamID;
$output["sessionid"] = (int)$SessionID;
$output["achievements"] = $p["achievements"];
$output["stats"] = $p["stats"];
echo json_encode($output);

$log->logInfo("Player connected : " . stripslashes($SteamID));
?>