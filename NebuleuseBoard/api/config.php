<?php

$g_config["Nebuleuse"] = 1;

//Database

$g_config["db_user"] = "orygin";

$g_config["db_pw"] = "*";

$g_config["db_host"] = "localhost";

$g_config["db_base"] = "orygin";

if(isset($_COOKIE["env"]) && $_COOKIE["env"] == "dev")
	$g_config["db_prefix"] = "dev_";
else
	$g_config["db_prefix"] = "neb_";


$g_config["steamapikey"] = "B4DFB6DD5893A1B9F4C284B46485EA80";



//Versior

$g_config["version_major"] = 1;

$g_config["version_minor"] = 0;

$g_config["version_patch"] = 2;



class NebuleuseError

{

	const NEBULEUSE_ERROR_NONE = 0;

	const NEBULEUSE_ERROR = 1; //Unspecified error

	const NEBULEUSE_ERROR_BANNED = 2;

	const NEBULEUSE_ERROR_MAINTENANCE = 3;//The service is on maintenance or offline

	const NEBULEUSE_ERROR_DISCONNECTED = 4;//The session timed out or never existed

	const NEBULEUSE_ERROR_OUTDATED = 5;//The server version is too different from the Nebuleuse version.

	const NEBULEUSE_ERROR_BADKEY = 6; //The key provided do not correspond with the key the server has for this game

	const NEBULEUSE_ERROR_PARSEFAILED = 7;

	const NEBULEUSE_ERROR_NON_BETA_PLAYER = 8;

}

define('PUN_ROOT', '../../forum/'); // Chemin relatif vers FluxBB (avec le slash final)
define('PUN_ADMIN', 1);
define('PUN_MOD', 2);
define('PUN_GUEST', 3);
define('PUN_MEMBER', 4);


define('UPDATES_DIR', '../../updates/');