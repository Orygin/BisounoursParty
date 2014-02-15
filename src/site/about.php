<?php
        require_once('start.php');
        
	include('./lang/' . USER_LANG . '/index.php');

	$TPL->set('LANG_ABOUT', file_get_contents('./lang/' . USER_LANG . '/static/about.md'));

	$TPL->set('PAGE_TITLE', $lang['LANG_SPEEDBARRE_ABOUT']);

	$TPL->set($lang);

	$TPL->parse('about.html');