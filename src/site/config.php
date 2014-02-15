<?php
	# MySQL
	define('MYSQL_HOST', 'localhost'); // Hôte
	define('MYSQL_PORT', '3306'); // Port
	define('MYSQL_USER', 'orygin'); // Utilisateur
	define('MYSQL_PASS', '*'); // Mot de passe
	define('MYSQL_DB', 'orygin'); // Base de données
	define('MYSQL_PREFIX', 'bp_'); // Préfixe des tables

	# FluxBB
	define('PUN_ROOT', 'forum/'); // Chemin relatif vers FluxBB (avec le slash final)
	define('PUN_ADMIN', 1);
	define('PUN_MOD', 2);
	define('PUN_GUEST', 3);
	define('PUN_MEMBER', 4);

	# Site
	define('SITE_LANG_DEFAULT', 'en');
	define('SITE_BASE_URL', 'http://www.bisounoursparty.com');