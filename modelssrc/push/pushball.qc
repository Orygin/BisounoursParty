$scale 0.75
$modelname "items/Pushball.mdl"
$model "studio" "Pushball_reference.smd"
$cdmaterials "models\items\push\"
$hboxset "default"
$hbox 0 "grenade"	    -16.00   -16.00   -16.00     16.00    16.00    16.00
$scale 0.75
$attachment "fuse" "ValveBiped.Bip01_R_Hand" -0.50 -2.50 1.70 rotate 0 0 0
$scale 0.75
$zbrush
$surfaceprop "ball"
$illumposition 0.000 0.000 0.000
$sequence idle "idle" ACT_VM_IDLE 1 fps 30.00
$collisionmodel "pushball_reference.smd" {

	$mass 10.0
	$inertia 1.50
	$damping 0.00
	$rotdamping 0.00
	$drag 0.30
}
