<?php
        require_once('start.php');
        
        if(isset($_GET['openid_ns']))
        {
            require_once('./libs/OpenId.php');
            $id = SteamSignIn::validate();
            
            if(!empty($id))
            {
                $users = $SQL->query("SELECT COUNT(*) as num FROM bpforum_users WHERE steamid64=".$id."");
                $count = $users->fetch();

                if($count['num'] < 1)
                { // On ne veux qu'un steamid par user
                    $db->query("UPDATE bpforum_users SET steamid64='".$id."' WHERE id='".$pun_user['id']."'");
                }
                header( 'Location: http://www.bisounoursparty.com/link/' );
            }
        }
        if(isset($_GET['unlink']))
        {
           $db->query("UPDATE bpforum_users SET steamid64='' WHERE id='".$pun_user['id']."'");
           header( 'Location: http://www.bisounoursparty.com/link/' );
        }
        
        if(!empty($pun_user['steamid64']))
        {
            $xml=simplexml_load_file('http://steamcommunity.com/profiles/' . $pun_user['steamid64'] . '/?xml=1');
            $TPL->set('PLAYER_NAME', $xml->steamID);
            $TPL->set('PLAYER_AVATAR', $xml->avatarMedium);
        }
        else
        {
            require_once('./libs/OpenId.php');
            $TPL->set('USER_CONNECTLINK', SteamSignIn::genUrl('http://www.bisounoursparty.com/link/'));
        }
        
	include('./lang/' . USER_LANG . '/link.php');

	$TPL->set($lang);

	$TPL->parse('link.html');
