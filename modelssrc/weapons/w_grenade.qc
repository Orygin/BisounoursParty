$cd "grenade\w_"
$scale 0.25
$modelname "weapons/w_grenade.mdl"
$model "studio" "Grenade_reference.smd"
$scale 0.31
$lod 40
{
	replacemodel 	"Grenade_reference" 	"Grenade_reference_lod1"
	replacematerial "sphere"  				"sphere_lod1"
}
$lod 100
{
	replacemodel 	"Grenade_reference" 	"Grenade_reference_lod1"
	replacematerial "sphere"  				"sphere_lod2"
}
$scale 0.25
$cdmaterials "models\weapons\w_grenade/"
$hboxset "default"
$hbox 0 "grenade"	    -32.00   -32.00   -32.00     32.00    32.00    32.00

$scale 1.00
$attachment "fuse" "ValveBiped.Bip01_R_Hand" -0.50 -2.50 1.70 rotate 0 0 0
$scale 0.25
$zbrush
$surfaceprop "grenade"
$illumposition 0.000 0.000 0.000
$sequence idle "idle" ACT_VM_IDLE 1 fps 30.00
$scale 1.00
$collisionmodel "Grenade_reference.smd" {

	$mass 10.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
