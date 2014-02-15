$cd "bigshotgun\v_"
$modelname "weapons/v_bigshotgun.mdl"
$body studio "Shotgun_reference.smd"
$cdmaterials "models\Weapons\v_hand\"
$cdmaterials "models\Weapons\v_bigshotgun\"
$cdmaterials "models\weapons\shell\"
$origin -4 10 2

$attachment "muzzle" "ValveBiped.Gun" -1.00 3.00 7.00 rotate -90 0 0
$attachment "1" "ValveBiped.Gun" -1.00 2.00 -14.00 rotate 0 0 0

$sequence idle01 "Idle01" loop fps 30 activity ACT_VM_IDLE 1 node 0
$sequence fire01 "Fire01" fps 30 snap activity ACT_VM_PRIMARYATTACK 1 { { event 6001 0 "2" } { event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" } } node 2
$sequence altfire "Altfire" fps 30 snap activity ACT_VM_SECONDARYATTACK 1 { { event 6001 0 "2" } { event 6001 0 "2" }  { event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" } } node 2
$sequence draw "Draw" fps 90 snap activity ACT_VM_DRAW 1 node 0
$sequence reload1 "Reload1" fps 30 activity ACT_SHOTGUN_RELOAD_START 1 node 0
$sequence reload2 "Reload2" fps 30 activity ACT_VM_RELOAD 1 node 0
$sequence reload3 "Reload3" fps 30 activity ACT_SHOTGUN_RELOAD_FINISH 1 node 0
$sequence pump "Pump" fps 30 activity ACT_SHOTGUN_PUMP 1 node 0
