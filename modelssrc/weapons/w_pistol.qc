$scale 1.4
$cd "pistol\w_"

$modelname "weapons/w_pistol.mdl"
$body studio "Pistol_reference.smd"

$cdmaterials "models\weapons\w_pistol\"
$hboxset "default"
$hbox 0 "ValveBiped.Bip01_R_Hand" 0.000  -1.500  -5.000  5.000  0.000  0.000
$hbox 0 "ValveBiped.Weapon_bone" -4.334  -1.435  -0.164  6.194  0.696  7.060

$scale 1.0
$attachment "muzzle" "ValveBiped.Weapon_bone" 19.00 0.90 2.90 rotate -2 0 -99
$scale 1.4
$surfaceprop "weapon"
$illumposition -0.013 0.491 -1.996

$sequence idle "idle" loop ACT_VM_IDLE 1 fps 30.00
$sequence attack1 "attack1" ACT_RANGE_ATTACK_RPG 1 fps 30.00 {
  { event 6001 0 "0" } 
  { event 3004 0 }
}

$collisionmodel "Pistol_reference.smd" {

	$mass 2.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
