<?php

$g_config["Nebuleuse"] = 1;

//Database

$g_config["db_user"] = "orygin";

$g_config["db_pw"] = "*";

$g_config["db_host"] = "localhost";

$g_config["db_base"] = "orygin";

$g_config["db_prefix"] = "neb_";


$g_config["log_level"] = 6; // DEBUG = 7, INFO = 6

//Version

$g_config["version_major"] = 1;

$g_config["version_minor"] = 0;

$g_config["version_patch"] = 2;


$g_config["Session_timeout"] = 1200;

$g_config["APIKey"] = "B4DFB6DD5893A1B9F4C284B46485EA80";

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