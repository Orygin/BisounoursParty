<?php
        require_once('start.php');
        
	include('./lang/' . USER_LANG . '/index.php');

	$TPL->set('PAGE_TITLE', $lang['LANG_SPEEDBARRE_TEAM']);

	$TPL->set($lang);
        
        $req = $SQL->prepare('SELECT    t.user_id AS USER_ID,
                                        t.poste AS USER_POSTE,
                                        u.username AS USER_NICK
                                        FROM ' . MYSQL_PREFIX . 'team t
                                        INNER JOIN bpforum_users u
                                            ON t.user_id = u.id');
        $req->execute();
        while($user = $req->fetch())
        {
          $TPL->block('users', array('USER_ID' => $user['USER_ID'],
                                        'USER_POSTE' => $user['USER_POSTE'],
                                        'USER_NICK' => $user['USER_NICK']));
        }
        

	$TPL->parse('team.html');