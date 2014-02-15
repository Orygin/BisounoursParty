$cd "uzi\w_"
$modelname "weapons/w_uzi.mdl"
$model "studio" "uzi_reference.smd"
$cdmaterials "models\weapons\w_uzi\"

$hboxset "default"

//$hbox 0 "ValveBiped.Bip01_R_Hand" 0.000  -1.500  -5.000  5.000  0.000  0.000
$hbox 0 "ValveBiped.Bip01_R_Hand"	     0.00   -5.00   -9.50    10.00    1.00    5.50

$attachment "muzzle" "ValveBiped.Weapon_bone" 8.00 -0.98 0.10 rotate 4 -1 -99
$attachment "1" "ValveBiped.Weapon_bone" 1.00 0.40 0.00 rotate 9 90 -85
$surfaceprop "weapon"
$illumposition -0.013 0.491 -1.996

$sequence idle "idle" loop ACT_VM_IDLE 1 fps 30.00
$sequence fire "attack1" ACT_RANGE_ATTACK_PISTOL 1 fps 4.50 {
  { event 6001 0 "0" }
  { event 3004 0 }
}

$collisionmodel "uzi_reference.smd" {

	$mass 2.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}