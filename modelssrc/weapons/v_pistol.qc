$cd "pistol\v_"

$modelname "weapons/v_pistol.mdl"
$body studio "pistol_reference.smd"
$cdmaterials "models\Weapons\V_hand\"
$cdmaterials "models\Weapons\V_Pistol\"

$attachment "muzzle" "muzzle" 0.00 0.00 7.00 rotate -90 90 0
$surfaceprop "default"


$sequence idle01 "idle01" loop ACT_VM_IDLE 1 rotate -90 fps 30.00 node Ready

$sequence fire "fire" snap  ACT_VM_PRIMARYATTACK 1 rotate -90 fps 28.00 node Fire {
  { event 6001 0 "0" } 
}
$sequence fire1 "fire" snap  ACT_VM_RECOIL1 1 rotate -90 fps 28.00 node Fire {
  { event 6001 0 "0" } 
}
$sequence fire2 "fire" snap  ACT_VM_RECOIL2 1 rotate -90 fps 28.00 node Fire {
  { event 6001 0 "0" } 
}
$sequence fire3 "fire" snap  ACT_VM_RECOIL3 1 rotate -90 fps 28.00 node Fire {
  { event 6001 0 "0" } 
}

$sequence draw "draw" snap rotate -90 fps 90 activity ACT_VM_DRAW 1 node Ready {
  { event AE_CL_PLAYSOUND 0 "Weapon_Pistol.Draw" }
}