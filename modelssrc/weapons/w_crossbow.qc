$cd "crossbow\w_"
$modelname "weapons/w_crossbow.mdl"
$body studio "HL2DM_crossbow_reference.smd"

$cdmaterials "models\weapons\w_crossbow\"
$hboxset "default"
$hbox 0 "ValveBiped.bone" -15.147  -2.464  0  15.069  7.705  26.967

$surfaceprop "weapon"
$illumposition 8.816 2.079 2.817
$sequence idle "idle" loop ACT_VM_IDLE 1 fps 30.00
$sequence attack1 "idle" fps 30.00
$collisionmodel "phymodel.smd" {

	$mass 10.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
