<?php
function addPlayer($steamid64)
{
	global $g_db;
	global $g_config;
	global $log;

	$Sth = $g_db->prepare("INSERT INTO ".$g_config["db_prefix"]."players VALUES (NULL,:steamid64,1,:gameversion,:gameversion);
							SET @pid = LAST_INSERT_ID();
			 				INSERT INTO ".$g_config["db_prefix"]."players_achievements (achievementid, playerid)
							(
								SELECT achievementid, @pid
								FROM ".$g_config["db_prefix"]."achievements
							);
							INSERT INTO ".$g_config["db_prefix"]."players_stats VALUES (@pid,0,0,0,0);
	 						");
	$Sth->bindParam(":steamid64", $steamid64);
	$Sth->bindParam(":gameversion", $g_config["gameVersion"]);
	$Sth->execute();
	$Sth->closeCursor();

	$log->logInfo("New player added : " . $steamid64);
	
	return getPlayer($steamid64);
}
function getPlayer($steamid64, $OnlyPlayer = false)
{
	global $g_db;
	global $g_config;

	$Sth = $g_db->prepare("SELECT * FROM ".$g_config["db_prefix"]."players WHERE steamid64 = :steamid64 LIMIT 1");
	$Sth->bindParam(":steamid64", $steamid64);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$p = $Sth->fetch();
	$Sth->closeCursor();
	if($p["steamid64"] == 0){ //Player don't exist, just add him.
		return addPlayer($steamid64);
	}
	if(!$OnlyPlayer){
		$p["achievements"] = getPlayerAchievements($p);
		$p["stats"] = getPlayerStats($p);
	}
	return $p;
}
function getPlayerAchievements($p)
{
	global $g_db;
	global $g_config;
	
	//LEFT JOIN ".$g_config["db_prefix"]."players
								//ON ".$g_config["db_prefix"]."players_achievements.playerid = neb_players.id

	$query = "SELECT achievementid, Progress, name, max FROM ".$g_config["db_prefix"]."players_achievements 
							LEFT JOIN ".$g_config["db_prefix"]."achievements
								using (achievementid)
							WHERE ".$g_config["db_prefix"]."players_achievements.playerid = :id";

	$Sth = $g_db->prepare($query);
	$Sth->bindParam(":id", $p["id"]);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$output = array();
	$i = 0;
	while($row = $Sth->fetch())	{
		$output[$i] = array('name' => $row["name"], 'max' => (int)$row["max"], "progress" => (int)$row["Progress"] );
		$i++;
	}
	return $output;
}
function getPlayerStats($p)
{
	global $g_db;
	global $g_config;
	
	$query = "SELECT * FROM ".$g_config["db_prefix"]."players_stats WHERE playerid = :id";
	$Sth = $g_db->prepare($query);
	$Sth->bindParam(":id", $p["id"]);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$output = array();
	$i = 0;

	while($row = $Sth->fetch())	{
		foreach($row as $name => $stat)
		{
			if($name != "playerid")
			{
				$output[$i] = array('Name' => $name, 'Value' => (int)$stat);
				$i++;
			}
		}
	}
	return $output;
}
function updatePlayerRank($p, $Rank)
{
	global $g_db;
	global $g_config;

	$Sth = $g_db->prepare("UPDATE ".$g_config["db_prefix"]."players SET rank=:rank WHERE id = :id LIMIT 1");
	$Sth->bindParam(":id", $p["id"]);
	$Sth->bindParam(":rank",$rank);
	$Sth->execute();
}
function updateAchievementProgress($p, $AchievementId, $AchievementProgress)
{
	global $g_db;
	global $g_config;

	$AchievementId++; // Id ingame start at 0 while the db starts at 1
	$Sth = $g_db->prepare("UPDATE ".$g_config["db_prefix"]."players_achievements SET Progress=:progress WHERE playerid = :playerid AND achievementid = :id LIMIT 1");
	$Sth->bindParam(":playerid", $p["id"]);
	$Sth->bindParam(":progress", $AchievementProgress);
	$Sth->bindParam(":id", $AchievementId);
	$Sth->execute();
}
function getPlayerSessionID($p)
{
	global $g_db;
	global $g_config;

	$Sth = $g_db->prepare("SELECT sessionId FROM ".$g_config["db_prefix"]."sessions WHERE playerid = :id");
	$Sth->bindParam(":id", $p["id"]);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$res = $Sth->fetch();
	if($res["sessionId"])
	{
		$Sth = $g_db->prepare("UPDATE ".$g_config["db_prefix"]."sessions SET lastAlive = NOW() WHERE playerid = :id");
		$Sth->bindParam(":id", $p["id"]);
		$Sth->execute();
	}	
	return $res["sessionId"];
}
function updateStats($p, $Stats)
{
	global $g_db;
	global $g_config;
	global $log;

	$data = json_decode($Stats,true);
	$log->logDebug("Stats update data : " . $data);

	foreach ($data["Stats"] as $stat) {
		$g_db->query("UPDATE ".$g_config["db_prefix"]."players_stats SET " . substr($g_db->quote($stat["name"]), 1, -1) . " = " . substr($g_db->quote($stat["value"]), 1, -1) . " WHERE playerid = " . substr($g_db->quote($p["id"]), 1, -1));
	}
	
	$Sth = $g_db->prepare("INSERT INTO ".$g_config["db_prefix"]."kills VALUES (:id,:x,:y,:z,:weapon,:map)");
	$Sth->bindParam(":id", $p["id"]);
	$Sth->bindParam(":map", $data["map"]);
	
	foreach($data["Kills"] as $kill)
	{
		$Sth->bindValue(":weapon", $kill["weapon"]);
		$Sth->bindValue(":x", $kill["x"]);
		$Sth->bindValue(":y", $kill["y"]);
		$Sth->bindValue(":z", $kill["z"]);
		$Sth->execute();
	}
}