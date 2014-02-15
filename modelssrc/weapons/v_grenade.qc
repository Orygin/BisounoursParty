$cd "grenade\v_"
$modelname "weapons/V_grenade.mdl"
$body studio "grenade_reference.smd"
$cdmaterials "models\Weapons\V_hand\"
$cdmaterials "models\weapons\w_grenade/"

$attachment "0" "ValveBiped.Bip01_R_Hand" 4.00 -4.00 0.00 rotate -0.00 0.00 0.00

$sequence holster1 "drawback" ACT_VM_PULLBACK_HIGH 1 fps 30.00 {
  { event 3900 4 }
}
$sequence throw "throw" snap  ACT_VM_THROW 1 fps 17.00 {
  { event 3005 1 }
}
$sequence idle01 "idle01" ACT_VM_IDLE 1 fps 30.00
$sequence draw "draw" snap activity ACT_VM_DRAW 1 fps 90.00
