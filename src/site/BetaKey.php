<?php
require_once('start.php');

function getPlayer($steamid64)
{
	global $SQL;
	$Sth = $SQL->prepare("SELECT * FROM neb_players WHERE steamid64 = :steamid64 LIMIT 1");
	$Sth->bindParam(":steamid64", $steamid64);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$p = $Sth->fetch();

	return $p;
}

if(isset($_POST["userid"]) AND isset($_POST["key"]))
{
	if(strlen((string)$_POST["userid"]) != 17)
		return;
	$sid = $_POST["userid"];
	$key = $_POST["key"];

	$query = $SQL->query('SELECT COUNT(*) as nbr FROM ' . MYSQL_PREFIX . 'betakey WHERE Betakey = "' . htmlspecialchars ($key, ENT_QUOTES) . '"');
	$keynbr = $query->fetch();
	if($keynbr >= 1)
	{
		$query = $SQL->prepare("
								INSERT INTO neb_players VALUES (NULL,:steamid64,2);
		 						INSERT INTO neb_players_stats VALUES (:steamid64,0,0,0,0);
		 						");
		$query->execute(array('steamid64' => $sid));

		$query = $SQL->prepare("SELECT * FROM neb_players WHERE steamid64 = :steamid64 LIMIT 1");
		$query->bindParam(":steamid64", $sid);
		$query->execute();
		$query->setFetchMode(PDO::FETCH_ASSOC);
		$p = $query->fetch();

		$query = $SQL->prepare("
				 				INSERT INTO neb_players_achievements (achievementid, playerid)
								(
									SELECT achievementid, :pid
									FROM neb_achievements
								)
		 						");
		$query->execute(array('pid' => $p["id"]));
		/*$query = $SQL->prepare('DELETE FROM ' . MYSQL_PREFIX . 'betakey WHERE Betakey = :key');
		$query->execute(array('key' => $key));*/
		$TPL->set("ADDED", '1');
	}
}

include('./lang/' . USER_LANG . '/betakey.php');

$TPL->set($lang);
$TPL->parse('Betakey.html');
?>