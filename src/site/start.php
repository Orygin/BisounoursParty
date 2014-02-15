<?php
	# Debug
	error_reporting(E_STRICT);

	# Configuration
	require_once('config.php');


	# FluxBB
	require_once(PUN_ROOT . 'include/common.php');

	# SQL
	try {
		$SQL = new PDO('mysql:host=' . MYSQL_HOST . ';port=' . MYSQL_PORT . ';dbname=' . MYSQL_DB, MYSQL_USER, MYSQL_PASS);
		$SQL->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
	}
	catch(Exception $error) {
		exit('MySQL Error (Database Connection) :<br /><blockquote>' . $error->getMessage() . '</blockquote>');
	}


	# TPL (Talus' TPL - http://www.talus-works.net/)
	include("./libs/Talus_TPL/Talus_TPL.php");
	$TPL = new Talus_TPL("./templates/tpl/", "./templates/tpl/cache/");
	$TPL->set('BASE_URL', SITE_BASE_URL);


	# Markdown (PHP Markdown - http://michelf.com/projets/php-markdown/)
	require_once('./libs/markdown.php');

        # Fonctions custom
        require_once ('./libs/functions.php');
        
        if(isset($_GET['switch_lang']))
        { // on ne crée un cookie que si l'on le demande
            if($_COOKIE['bp_lang'] == 'fr'){
                setcookie('bp_lang', 'en', time() + 365*24*3600, null, null, false, true);
                define('USER_LANG', 'en');
                }
            else{
                setcookie('bp_lang', 'fr', time() + 365*24*3600, null, null, false, true);
                define('USER_LANG', 'fr');
                }
                header("Location: " . $_SERVER['HTTP_REFERER']);
        }
            
	# Langue
        if(!empty($_COOKIE['bp_lang']))
        {
            define('USER_LANG', $_COOKIE['bp_lang']);
        }
        else if($pun_user['language'] == "French")
        {
            define('USER_LANG', 'fr');
        }
        else
        {// On veux de toute facon être en anglais si on est pas FR
            define('USER_LANG', 'en');

        }

        if($pun_user['username'] == "Guest")
        {
            define('USER_CONNECTED', false);
            $TPL->set('ISCONNECTED', '0');
        }
        else{
            define('USER_CONNECTED', true);
            $TPL->set('ISCONNECTED', '1');
            $TPL->set('USER_ID', $pun_user['id']);
            $TPL->set('USER_NICK', $pun_user['username']);
            $TPL->set('USER_OUTTOKEN', pun_hash($pun_user['id'].pun_hash(get_remote_address())));
            $TPL->set('USER_GROUP_ID', $pun_user['group_id']);
            if(empty($pun_user['steamid64']))
            {
                $TPL->set ('USER_CLOUDLINKED', '0');
            }
            else
            {
                $TPL->set ('USER_CLOUDLINKED', '1');
                $TPL->set ('USER_STEAMID64', $pun_user['steamid64']);
            }
            if(IsAdmin($pun_user)){
                $TPL->set('USER_ADMIN', '1');
                $TPL->set('USER_CAN_MODERATE', '1');
            }
            else if ($pun_user['group_id'] == 2 )
                $TPL->set('USER_CAN_MODERATE', '1');
        }
        
        $TPL->set('USER_LANG', USER_LANG);
        
        if(USER_LANG == "fr")
            define('USER_LANGI', 'en'); 
        else
            define('USER_LANGI', 'fr');
        $TPL->set('USER_LANGI', USER_LANGI);
        
	include('./lang/' . USER_LANG . '/general.php');
	include('./lang/' . USER_LANG . '/urls.php');

	