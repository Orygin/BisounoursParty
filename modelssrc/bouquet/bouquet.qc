$scale 1.00
$cd "."
$modelname items/crossbowrounds
$model "body" "bouquet_ref.smd"
$cdmaterials "models\weapons\"

$surfaceprop "item"
$sequence idle "bouquet_anim" loop fps 30.00
$collisionmodel "bouquet_phy" {

	$concave
	$mass 5.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
