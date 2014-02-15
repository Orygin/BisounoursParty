<?php
include('config.php');

if(!isset($_POST['SteamId']))
{
	echo NebuleuseError::NEBULEUSE_ERROR;;
	$log->logError("GetSteamAvatar : steamid not transmitted");
	exit();
}

$id = htmlspecialchars($_POST['SteamId']);

$file = file_get_contents("http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=" . $g_config["APIKey"] . "&steamids=" . $id);
$json = json_decode($file, true);

header('Content-type: image/jpeg');

readfile($json["response"]["players"][0]["avatarfull"]);
?>
