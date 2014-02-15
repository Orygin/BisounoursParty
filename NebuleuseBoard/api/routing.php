<?php //Routing
Flight::route('/', function(){
    echo "/api/getSettings/ - returns the complete settings of the current nebuleuse\n";
    echo "/api/setSettings/ - POST settings - sets the new settings\n";

    echo "/api/getMirrors/ - returns the complete mirror list\n";

    echo "/api/getUpdates/ - returns the complete update list\n";
    echo "/api/addUpdate/ - POST version, size, log - adds an update\n";
    echo "/api/deleteUpdate/ - POST version - deletes an update\n";
    echo "/api/setActiveVersion/ - POST version - sets the current version\n";

    echo "/api/getPlayers/ - returns the complete player list\n";
    echo "/api/getPlayers/#page - returns the players from (#page-1)*30 to #page*30\n";
    echo "/api/getPlayer/#id - returns a single player's info\n";
    echo "/api/deletePlayer/ - POST id - deletes a player from database\n";
    echo "/api/setPlayerRank/ - POST id,rank - updates a player's rank\n";

    echo "/api/getStats/ - returns the complete stats of the current nebuleuse\n";

    echo "/api/getAchievements/ - returns a complete achievements list\n";
    echo "/api/getAchievement/#id - returns a single achievement's info\n ";
    echo "/api/setAchievement/ - POST id,devname,fullname,fulldesc,max - saves the achievement\n";
    echo "/api/addAchievement/ - POST id,devname,fullname,fulldesc,max - adds an achievement\n";
    echo "/api/deleteAchievement/ - POST id - deletes the achievement\n";

    echo "/api/clearSessions/ - empties the session table\n";
    echo "api/switchChannel/ - Toggles the dev environment\n";
});

Flight::route('/getSettings/', function(){
	$cfg = Flight::get('config');
	$output["gameVersion"] = (int)$cfg["gameVersion"];
	$output["updaterVersion"] = (int)$cfg["updaterVersion"];
	$output["gameName"] = $cfg["gameName"];
	$output["gameKey"] = $cfg["gameKey"];
	$output["sessionTimeout"] = (int)$cfg["sessionTimeout"];
	$output["status"] = $cfg["maintenance"] ? "disabled" : "enabled";
	if(isset($_COOKIE["env"]) && $_COOKIE["env"] == "dev")
		$output["channel"] = "dev";
	else
		$output["channel"] = "release";

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/setSettings/', function() {
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$Sth = $db->prepare("UPDATE " . $cfg["db_prefix"] . "config SET value=:value WHERE name=:name");
	foreach ($_POST as $key => $value) {
		$Sth->bindParam('value', $value);
		$Sth->bindParam('name', $key);
		$Sth->execute();
	}
});
Flight::route('/getMirrors/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$output["mirrors"] = array();

	$Sth = $db->prepare("SELECT * FROM " . $cfg["db_prefix"] . "mirrors");
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$i = 0;
	while($row = $Sth->fetch()){
		$output["mirrors"][$i]["name"] = $row['Name'];
		$output["mirrors"][$i]["address"] = $row['Address'];
		$f = get_headers($row["Address"] . $cfg['gameVersion'] . '.update');
		$output["mirrors"][$i]["responding"] = ($f[0] == 'HTTP/1.1 404 Not Found') ? false : true;//file_exists($row["Address"] . $cfg['gameVersion'] . '.update') ? true : false;
		$i++;
	}

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/getUpdates/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$output["updates"] = array();

	$Sth = $db->prepare("SELECT version,log,date,size FROM " . $cfg["db_prefix"] . "updates");
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);

	$i = 0;
	while($row = $Sth->fetch()) {
		$output["updates"][$i]["version"] = (int)$row["version"];
		$output["updates"][$i]["size"] = (int)$row["size"];
		$output["updates"][$i]["date"] = $row["date"];
		$output["updates"][$i]["log"] = utf8_encode($row["log"]);
		$output["updates"][$i]["fullyPropagated"] = true;
		$i++;
	}

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/setActiveVersion/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');
	
	$version = $_POST["version"];

	$Sth = $db->prepare("UPDATE " . $cfg["db_prefix"] . "config SET value=:version WHERE name = 'gameVersion'");
	$Sth->bindParam(":version", $version);
	$Sth->execute();
});
Flight::route('/addUpdate/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$version = $_POST["version"];
	$size = $_POST["size"];
	$log = $_POST["log"];

	$Sth = $db->prepare("INSERT INTO " . $cfg["db_prefix"] . "updates (version, log, size) VALUES (:version, :log, :size)");
	$Sth->bindParam(":version", $version);
	$Sth->bindParam(":size", $size);
	$Sth->bindParam(":log", $log);
	$Sth->execute();
});
Flight::route('/deleteUpdate/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$version = $_POST["version"];

	$Sth = $db->prepare("DELETE FROM " . $cfg["db_prefix"] . "updates WHERE version=:version");
	$Sth->bindParam(":version", $version);
	$Sth->execute();
});
Flight::route('/getPlayers/(@page)', function($page){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$output["players"] = array();

	$Sth = $db->prepare("SELECT COUNT(*) AS count FROM " . $cfg["db_prefix"] . "players");
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$res = $Sth->fetch();
	$output["count"] = $res["count"];

	if($page === NULL) // If $page is not null it is certified by flight to be a number between 0 and 999
		$Sth = $db->prepare("SELECT  " . $cfg["db_prefix"] . "players.id,  " . $cfg["db_prefix"] . "players.rank, " . $cfg["db_prefix"] . "players.steamid64, 
								 " . $cfg["db_prefix"] . "players_stats.timePlayed
								FROM " . $cfg["db_prefix"] . "players
								LEFT JOIN  " . $cfg["db_prefix"] . "players_stats 
								ON  " . $cfg["db_prefix"] . "players.id =  " . $cfg["db_prefix"] . "players_stats.playerid");
	else
		$Sth = $db->prepare("SELECT  " . $cfg["db_prefix"] . "players.id,  " . $cfg["db_prefix"] . "players.rank, " . $cfg["db_prefix"] . "players.steamid64, 
								 " . $cfg["db_prefix"] . "players_stats.timePlayed
								FROM " . $cfg["db_prefix"] . "players
								LEFT JOIN  " . $cfg["db_prefix"] . "players_stats 
								ON  " . $cfg["db_prefix"] . "players.id =  " . $cfg["db_prefix"] . "players_stats.playerid
								LIMIT " . ($page-1)*30 . "," . $page*30);

	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$i = 0;
	while($row = $Sth->fetch()) {
		$output["players"][$i]["id"] = (int)$row["id"];
		$output["players"][$i]["rank"] = (int)$row["rank"];
		$output["players"][$i]["steamId"] = $row["steamid64"];
		$output["players"][$i]["timePlayed"] = $row["timePlayed"];

		$i++;
	}
	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/getPlayer/@id', function($id){ // In this case we redirect to steam's infos
	$cfg = Flight::get('config');
	echo (file_get_contents('http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=' . $cfg["steamapikey"] . '&steamids='. $id));
});
Flight::route('/getStats/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$req = $db->prepare('SELECT SUM(timePlayed) AS timeTotal, SUM(kills) AS killsTotal, SUM(mapsLoaded) AS mapsTotal FROM ' . $cfg["db_prefix"] . 'players_stats');
	$req->execute();
	$temp = $req->fetch();
	$output["stats"]["totalPlayTime"] = (int)$temp['timeTotal'];
	$output["stats"]["gameCount"] = (int)$temp['mapsTotal'];
	$output["stats"]["killCount"] = (int)$temp['killsTotal'];

	$req = $db->prepare('SELECT COUNT( * ) FROM ' . $cfg["db_prefix"] . 'players');
	$req->execute();
	$temp = $req->fetch();
	$output["stats"]["playerNumber"] = (int)$temp['0'];

	$req = $db->prepare('SELECT SUM(Progress) AS BongSmoked FROM ' . $cfg["db_prefix"] . 'players_achievements WHERE achievementid = 4');
	$req->execute();
	$temp = $req->fetch();
	$output["stats"]["bongSmoked"] = (int)$temp['BongSmoked'];

	$req = $db->prepare('SELECT COUNT( * ) FROM ' . $cfg["db_prefix"] . 'sessions');
	$req->execute();
	$temp = $req->fetch();
	$output["stats"]["playersOnline"] = (int)$temp['0'];

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/getAchievements/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$output["achievements"] = array();

	$Sth = $db->prepare("SELECT * FROM " . $cfg["db_prefix"] . "achievements");
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	$i = 0;
	while($row = $Sth->fetch()){
		$output["achievements"][$i]["icon"] = "img/avatar.jpg";
		$output["achievements"][$i]["id"] = (int)$row["achievementid"];
		$output["achievements"][$i]["name"] = $row["name"];
		$output["achievements"][$i]["progressMax"] = (int)$row["max"];
		$output["achievements"][$i]["progress"] = (int)$row["max"]/2;
		$output["achievements"][$i]["fullName"] = $row["fullName"];
		$output["achievements"][$i]["fullDesc"] = $row["fullDesc"];
		$i++;
	}

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/getAchievement/@id', function($id){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$Sth = $db->prepare("SELECT * FROM " . $cfg["db_prefix"] . "achievements WHERE achievementid=:id LIMIT 1");
	$Sth->bindParam(":id", $id);
	$Sth->execute();
	$Sth->setFetchMode(PDO::FETCH_ASSOC);
	while($row = $Sth->fetch()){
		$output["achievement"]["icon"] = "img/avatar.jpg";
		$output["achievement"]["id"] = (int)$row["achievementid"];
		$output["achievement"]["name"] = $row["name"];
		$output["achievement"]["progressMax"] = (int)$row["max"];
		$output["achievement"]["progress"] = (int)$row["max"]/2;
		$output["achievement"]["fullName"] = $row["fullName"];
		$output["achievement"]["fullDesc"] = $row["fullDesc"];
	}

	$output = json_encode($output, true);
	echo ")]}',\n" . $output;
});
Flight::route('/deletePlayer/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');
	$id = $_POST["id"];

	$Sth = $db->prepare("DELETE FROM " . $cfg["db_prefix"] . "players WHERE id=:id LIMIT 1;
							DELETE FROM " . $cfg["db_prefix"] . "players_stats WHERE id=:id LIMIT 1;
							DELETE FROM " . $cfg["db_prefix"] . "players_achievements WHERE id=:id LIMIT 1;");

	$Sth->bindParam(":id", $id);
	$Sth->execute();
});
Flight::route('/setPlayerRank/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');
	$id = $_POST["id"];
	$rank = $_POST["rank"];

	$Sth = $db->prepare("UPDATE " . $cfg["db_prefix"] . "players SET rank=:rank WHERE id=:id");
	$Sth->bindParam(":id", $id);
	$Sth->bindParam(":rank", $rank);
	$Sth->execute();
});
Flight::route('/setAchievement/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$id = $_POST["id"];
	$devname = $_POST['devname'];
	$fullname = $_POST['fullname'];
	$fulldesc = $_POST['fulldesc'];
	$progress = $_POST['progress'];
	$max = $_POST['max'];

	$Sth = $db->prepare("UPDATE " . $cfg["db_prefix"] . "achievements SET name=:devname, max=:max, fullName=:fullname, fullDesc=:fulldesc WHERE achievementid=:id");
	$Sth->bindParam(':devname', $devname);
	$Sth->bindParam(':max', $max);
	$Sth->bindParam(':fullname', $fullname);
	$Sth->bindParam(':fulldesc', $fulldesc);
	$Sth->bindParam(':id', $id);

	$Sth->execute();
});
Flight::route('/addAchievement/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$devname = $_POST['devname'];
	$fullname = $_POST['fullname'];
	$fulldesc = $_POST['fulldesc'];
	$progress = $_POST['progress'];
	$max = $_POST['max'];

	$Sth = $db->prepare("INSERT INTO " . $cfg["db_prefix"] . "achievements (achievementid,name,max,fullName,fullDesc) VALUES (NULL,:devname, :max, :fullname, :fulldesc);
						INSERT INTO ".$g_config["db_prefix"]."players_achievements (achievementid, playerid)
							(
								SELECT LAST_INSERT_ID(), id
								FROM ".$g_config["db_prefix"]."players
							);");
	
	$Sth->bindParam(':devname', $devname);
	$Sth->bindParam(':max', $max);
	$Sth->bindParam(':fullname', $fullname);
	$Sth->bindParam(':fulldesc', $fulldesc);

	$Sth->execute();
});
Flight::route('/deleteAchievement/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$id = $_POST['id'];

	$Sth = $db->prepare("DELETE FROM " . $cfg["db_prefix"] . "achievements WHERE achievementid=:id;
							DELETE FROM " . $cfg["db_prefix"] . "players_achievements WHERE achievementid=:id");
	$Sth->bindParam(':id', $id);

	$Sth->execute();
});
Flight::route('/clearSessions/', function(){
	$db = Flight::get('db');
	$cfg = Flight::get('config');

	$Sth = $db->prepare("DELETE FROM " . $cfg["db_prefix"] . "sessions");
	$Sth->execute();
});
Flight::route('/exportDatabase/', function(){
	$cfg = Flight::get('config');

	system("mysqldump -u " . $cfg["db_user"] . " -p" . $cfg["db_pw"] . " " . $cfg["db_base"] . " \
	" . $cfg["db_prefix"] . "players \
	" . $cfg["db_prefix"] . "players_stats \
	" . $cfg["db_prefix"] . "players_achievements \
	" . $cfg["db_prefix"] . "kills \
	" . $cfg["db_prefix"] . "config \
	" . $cfg["db_prefix"] . "achievements \
	" . $cfg["db_prefix"] . "sessions \
	" . $cfg["db_prefix"] . "mirrors > db.sql", $ret);

	header("Content-Disposition: attachment; filename=db.sql");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Description: File Transfer");

	echo file_get_contents("db.sql");
});
Flight::route('/switchChannel/', function(){
	if(isset($_COOKIE["env"]) && $_COOKIE["env"] == "dev")
		setcookie("env", "release", time()+60*60*24*30, '/');
	else
		setcookie("env", "dev", time()+60*60*24*30, '/');
});