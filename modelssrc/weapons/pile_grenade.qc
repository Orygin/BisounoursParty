$cd "grenade\w_"
$scale 0.25
$modelname "items/ammo_grenade.mdl"
$model "studio" "pile_grenade.smd"
$lod 40
{
	replacemodel 	"pile_grenade" 	"pile_grenade_lod"
	replacematerial "sphere"  				"sphere_lod1"
}
$lod 100
{
	replacemodel 	"pile_grenade" 	"pile_grenade_lod"
	replacematerial "sphere"  				"sphere_lod2"
}
$cdmaterials "models\weapons\w_grenade/"

$surfaceprop "weapon"
$sequence idle "pile_grenade" ACT_VM_IDLE 1 fps 30.00
$collisionmodel "pile_grenade_lod" {

	$mass 10.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}