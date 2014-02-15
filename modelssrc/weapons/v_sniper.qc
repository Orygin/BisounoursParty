$cd "sniper\v_"
$modelname "weapons/v_sniper.mdl"
$body studio "sniper_reference.smd"

$origin -16 1 -1.6
$cdmaterials "models\Weapons\v_sniper\"
$cdmaterials "models\Weapons\V_hand\"

$attachment "MUZZLE" "Bone03" -0.97 39.00 -5.40 rotate 0 90 0

$sequence idle "idle" loop ACT_VM_IDLE 1 rotate -90 fps 10.00

$sequence fire1 "fire" snap ACT_VM_PRIMARYATTACK 1 rotate -90 fps 20.00 {
  { event AE_MUZZLEFLASH 0 "357 muzzle" }
}
$sequence fire2 "fire1" snap ACT_VM_PRIMARYATTACK 1 rotate -90 fps 20.00 {
  { event AE_MUZZLEFLASH 0 "357 muzzle" }
}

$sequence draw "draw" snap rotate -90 fps 30.00 activity ACT_VM_DRAW 1 node Ready