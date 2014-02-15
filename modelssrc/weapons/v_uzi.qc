$cd "uzi\v_"
$modelname "weapons/v_uzi.mdl"
$body studio "smg1_reference.smd"
$cdmaterials "models\Weapons\v_hand\"
$cdmaterials "models\Weapons\v_uzi\"
$origin 1 0.4 0

$attachment "muzzle" "ValveBiped.muzzle" 0.00 0.50 0.00 rotate 0 0 0
$attachment "1" "ValveBiped.eject" -1.00 0.00 -0.00 rotate -0.00 90.00 180.00


//$sequence idle01 "idle01" loop ACT_VM_IDLE 1 rotate 90 fps 30.00
$sequence idle01 "idle01" loop fps 30 activity ACT_VM_IDLE 10 node Ready rotate 90
$sequence fire01 "fireempty" snap activity ACT_VM_PRIMARYATTACK 10 rotate 90 fps 27.00 { { event 6001 0 "0" } }
$sequence fire02 "fire" snap activity ACT_VM_PRIMARYATTACK 10 rotate 90 fps 27.00 { { event 6001 0 "0" } }

$sequence fire03 "fire1" snap activity ACT_VM_RECOIL1 10 rotate 90 fps 27.00 { { event 6001 0 "0" } }

$sequence fire04 "fire2" snap activity ACT_VM_RECOIL2 10 rotate 90 fps 27.00 { { event 6001 0 "0" } }

$sequence fire05 "fire3" snap activity ACT_VM_RECOIL3 10 rotate 90 fps 27.00 { { event 6001 0 "0" } }

//$sequence draw "draw" fps 90.00 snap activity ACT_VM_DRAW 1 node Ready

$sequence draw "draw" rotate 90 fps 60.00 snap ACT_VM_DRAW 1

$sequence dryfire "fire" fps 30 snap activity ACT_VM_DRYFIRE 1 node Fire rotate 90
$sequence idletolow "fire" fps 30 activity ACT_VM_LOWERED_TO_IDLE 1 transition Ready Low rotate 90
$sequence lowtoidle "fire" fps 30 activity ACT_VM_IDLE_TO_LOWERED 1 transition Low Ready rotate 90
$sequence lowidle "fire" fps 30 loop activity ACT_VM_IDLE_LOWERED 1 node Low rotate 90

//$sequence reload "Reload" ACT_VM_RELOAD 1 snap rotate 90 fps 30.00 node Ready 
//$sequence reload "Reload" snap activity ACT_VM_RELOAD 10 rotate 90 fps 30.00
$sequence reload "Reload" fps 30 activity ACT_VM_RELOAD 1 node Ready rotate 90
//{
//{ event AE_CL_PLAYSOUND 1 "Weapon_SMG1.Reload1" }
//{ event AE_CL_PLAYSOUND 16 "Weapon_SMG1.Reload2" }
//{ event AE_CL_PLAYSOUND 27 "Weapon_SMG1.Reload3" }
//}
//$sequence reload "Reload" fps 30 snap activity ACT_VM_RELOAD 1 node Ready rotate 90