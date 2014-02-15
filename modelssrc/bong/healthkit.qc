$cd "."
$scale 0.17
$origin 0 0 -10
$modelname "items/HealthKit.mdl"
$model "body" "bong_ref.smd"
$cdmaterials "models/items"
$surfaceprop "glass"
$sequence "idle" "idle" loop fps 30.00
$collisionmodel "phymodel" {

	$concave
	$mass 50.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}