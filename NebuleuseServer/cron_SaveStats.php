<?php
require("head.php");

$stats["date"] = Date("ymd");

$TotalTimePlayed = 0; // Time played
$req = $g_db->prepare('SELECT SUM(timePlayed) AS timeTotal FROM `neb_players_stats`');
$req->execute();
$temp = $req->fetch();
$stats["timePlayed"] = $temp['timeTotal'];
//Kills
$req = $g_db->prepare('SELECT SUM(kills) AS killsTotal FROM `neb_players_stats`');
$req->execute();
$temp = $req->fetch();
$stats["killsTotal"] = $temp['killsTotal'];
//PlayerCount
$req = $g_db->prepare('SELECT COUNT( * ) FROM `neb_players`');
$req->execute();
$temp = $req->fetch();
$stats["playerCount"] = $temp['0'];
//mapscount
$req = $g_db->prepare('SELECT SUM(mapsLoaded) AS mapsTotal FROM `neb_players_stats`');
$req->execute();
$temp = $req->fetch();
$stats["mapsTotal"] = $temp['mapsTotal'];
//bongcount
$req = $g_db->prepare('SELECT SUM(Progress) AS BongSmoked FROM neb_players_achievements WHERE achievementid = 4');
$req->execute();
$temp = $req->fetch();
$stats["bongSmoked"] = $temp['BongSmoked'];

$output["Stats"] = $stats;
$s = json_encode($output,true);

file_put_contents('stats/' . $stats["date"] . '.json',$s);
$log->logInfo("Saved stats for " . $stats["date"]);