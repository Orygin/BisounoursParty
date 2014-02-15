$cd "."
$scale 0.15
$origin 0 0 -8
$modelname "healthvial.mdl"
$model "body" "vodka_ref.smd"
$cdmaterials "models/items"
$surfaceprop "glass"
$sequence "idle" "idle" loop fps 30.00
$collisionmodel "phymodel" {

	$concave
	$mass 25.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}