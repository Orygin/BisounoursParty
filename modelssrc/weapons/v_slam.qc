$cd "slam\v_"
$modelname "weapons/v_slam.mdl"
$origin 0 0 -1
$body studio "v_slam_reference.smd"
$cdmaterials "models\Weapons\v_hand\"
$cdmaterials "models\Weapons\v_slam\"

$sequence tripmine_idle "tripmine_idle" ACT_SLAM_TRIPMINE_IDLE 1 fps 30.00
$sequence tripmine_draw "tripmine_draw" fps 90.00 snap activity ACT_SLAM_TRIPMINE_DRAW 1
$sequence tripmine_attach1 "tripmine_attach1" snap activity ACT_SLAM_TRIPMINE_ATTACH 1 fps 30.00
$sequence tripmine_attach2 "tripmine_attach2" snap activity ACT_SLAM_TRIPMINE_ATTACH2 1 fps 30.00
