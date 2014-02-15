<?php
require_once 'flight/Flight.php';

require_once 'config.php';
require_once 'db.php';
require_once 'routing.php';

require_once(PUN_ROOT . 'include/common.php');

if($pun_user['g_id'] != PUN_ADMIN) // Relatif au fluxbb se trouvant plus haut. on l'utilise pour gérer les perms
	exit();

Flight::start();
?>
