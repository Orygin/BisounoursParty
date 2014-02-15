// LAISSER $COLLISIONMODEL EN COMMENTAIRE OU SAUVER LE .PHY AVANT DE COMPILER

$cd "bigshotgun\w_"
$modelname "weapons/w_bigshotgun.mdl"
$model "studio" "shotgun_reference.smd"
//$lod 25
// {
//  replacemodel "shotgun_reference" "lod1_shotgun_reference"
//}
$cdmaterials "models\weapons\v_bigshotgun\"
$hboxset "default"
$hbox 0 "ValveBiped.Bip01_R_Hand" 0.000  -1.500  -5.000  5.000  0.000  0.000
$hbox 0 "ValveBiped.Weapon_bone" -4.140  -0.661  -3.836  25.847  1.720  7.981
// Model uses material "w_shotgun.vmt"
$attachment "muzzle" "ValveBiped.Weapon_bone" 20.00 0.00 -4.00 rotate 4 0 -95
$attachment "1" "ValveBiped.Weapon_bone" 3.00 -0.50 -2.00 rotate 0 -90 -95
$surfaceprop "weapon"
$illumposition 0.686 -0.312 -0.323
$sequence idle1 "idle1" loop ACT_VM_IDLE 1 fps 30.00
$sequence fire "fire" ACT_RANGE_ATTACK_SHOTGUN 1 fps 5.00 {
  { event 6001 0 "2" }
  { event 3004 0 }
}
$sequence altfire "fire" ACT_RANGE_ATTACK_SHOTGUN 1 fps 5.00 {
  { event 6001 0 "2" }
  { event 6001 0 "2" }
  { event 3004 0 }
}

$sequence draw "draw" ACT_VM_DRAW 1 fps 30.00
$sequence holster "holster" ACT_VM_HOLSTER 1 fps 30.00
//$collisionmodel "phymodel.smd" {
//
//	$concave
//	$mass 10.0
//	$inertia 1.00
//	$damping 0.00
//	$rotdamping 0.00
//}
