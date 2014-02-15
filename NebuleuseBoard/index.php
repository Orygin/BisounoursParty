<?php
define("PUN_ROOT", "../forum/");
define('PUN_ADMIN', 1);
require_once(PUN_ROOT . 'include/common.php');

if($pun_user['g_id'] != PUN_ADMIN) // Relatif au fluxbb se trouvant plus haut. on l'utilise pour gérer les perms
	exit();
?>
<!DOCTYPE html>
<html ng-app="ngView">
	<head>
		<title>Nebuleuse administration</title>
		<link href="bootstrap.css" rel="stylesheet">
		<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.1.3/angular.min.js" type="text/javascript"></script> <!-- Angular JS -->
		<script src="ui-bootstrap.min.js" type="text/javascript"></script>
		<script src="board.js" type="text/javascript"></script>
		<link rel="stylesheet" type="text/css" href="stylesheet.css">
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
		<meta name="description" content="Nebuleuse administration panel" /> 
	</head>
	<body ng-controller="MainCtrl">
		<div class="header">
			<div class="HeaderText"><a ng-href="#/">Nebuleuse</a></div>
			<div class="Cp">
				<img class="GameLogo" src="img/logo.jpg" alt="Game Logo" />
				<p class="CPText"><span class="CPTextGameName">{{settings.gameName}}</span><br/><span class="CPTextSubGameName"><span class="StatNumber">{{stats.playersOnline}}</span> players online</span></p>
			</div>
			<div class="HeaderSubMenu">
				<a ng-href="#/Achievements/">Achievements</a> | <a ng-href="#/Updates/">Updates</a> | <a ng-href="#/Players/">Players</a> | <a ng-href="#/Settings/">Settings</a>
			</div>
			<!--<div class="HeaderSubMenuMotd">
				<span class="StatNumber">128</span> Players registered <span class="StatNumber">5</span> Players online <span class="StatNumber">12</span> servers online
			</div>-->
		</div>
  		<alert ng-repeat="alert in alerts" type="alert.type" close="closeAlert($index)">{{alert.msg}}</alert>
		<div id="content" ng-view>
		</div>
	</body>
</html>
