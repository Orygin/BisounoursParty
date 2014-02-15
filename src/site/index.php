<?php
        require_once('start.php');
        include('./lang/'. USER_LANG . '/stats.php');
	include('./lang/' . USER_LANG . '/index.php');

	$TPL->set('LANG_INDEX_WELCOME', file_get_contents('./lang/' . USER_LANG . '/static/welcome.md'));
	$TPL->set('LANG_INDEX_INTRODUCTION', file_get_contents('./lang/' . USER_LANG . '/static/introduction.md'));

	$TPL->set('PAGE_TITLE', $lang['LANG_INDEX_PAGE_TITLE']);

	$TPL->set($lang);
        
        $TotalTimePlayed = 0;
        $req = $SQL->prepare('SELECT SUM(timePlayed) AS timeTotal FROM `neb_players_stats`');
        $req->execute();
        $temp = $req->fetch();
        $TotalTimePlayed = $temp['timeTotal'];
        $hours = floor($TotalTimePlayed /3600);
		$minutes = floor($TotalTimePlayed /60) - ($hours*60);
        $seconds = $TotalTimePlayed%60;
        $TPL->set('STAT_TOTALPLAYED_M', $minutes);
        $TPL->set('STAT_TOTALPLAYED_S', $seconds);
        $TPL->set('STAT_TOTALPLAYED_H', $hours);
        
        $req = $SQL->prepare('SELECT SUM(kills) AS killsTotal FROM `neb_players_stats`');
        $req->execute();
        $temp = $req->fetch();
        $TPL->set('STAT_KILLCOUNT', $temp['killsTotal']);
        
        $req = $SQL->prepare('SELECT COUNT( * )
                                FROM `neb_players`');
        $req->execute();
        $temp = $req->fetch();
        $TPL->set('STAT_PLAYERCOUNT', $temp['0']);
        
        $req = $SQL->prepare('SELECT SUM(mapsLoaded) AS mapsTotal FROM `neb_players_stats`');
        $req->execute();
        $temp = $req->fetch();
        $TPL->set('STAT_GAMECOUNT', $temp['mapsTotal']);

        $req = $SQL->prepare('SELECT SUM(Progress) AS BongSmoked FROM neb_players_achievements WHERE achievementid = 4');
        $req->execute();
        $temp = $req->fetch();
        $TPL->set('STAT_BONGCOUNT', $temp['BongSmoked']);
        
        $req = $SQL->prepare('SELECT COUNT( * )
                                FROM neb_sessions');
        $req->execute();
        $temp = $req->fetch();
        $TPL->set('STAT_ONLINE', $temp['0']);
        
	$TPL->parse('index.html');