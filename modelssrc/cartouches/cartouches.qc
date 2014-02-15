$scale 0.15
$cd "."
$modelname items/BoxBuckshot
$model "body" "cartouches.smd"
$cdmaterials "models/items"
$origin 0 0 -3.5

$sequence "shotshell" "cartouches_anim" rotate 0 fps 30.00
$surfaceprop "item"
//$illumposition 0.004 0.003 5.785
$sequence idle "cartouches_anim" loop fps 5.00
$collisionmodel "cartouches" {

	$concave
	$mass 5.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
