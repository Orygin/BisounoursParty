$scale 1.4
$cd "sniper\w_"
$modelname "weapons/w_sniper.mdl"
$model "studio" "shotgun_reference.smd"

$cdmaterials "models\weapons\w_sniper\"

$scale 1.0
$attachment "muzzle" "ValveBiped.Weapon_bone" 32.00 0.97 -1.30 rotate 6 0 -90

$surfaceprop "weapon"

$illumposition 0.686 -0.312 -0.323

$sequence idle1 "shotgun_reference" loop ACT_VM_IDLE 1 fps 30.00
$sequence fire "shotgun_reference" ACT_RANGE_ATTACK_RPG 1 fps 5.00 {
  { event AE_NPC_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
  { event 3004 0 }
}

$collisionmodel "shotgun_reference.smd" {

	$concave
	$mass 10.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
