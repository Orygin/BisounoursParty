$cd "."
$scale 1.0
$modelname "items/nounours_noragdoll.mdl"
$model "body" "nounours.smd"
$cdmaterials "models/items"

$texturegroup skinfamilies
{
    { "nounours_rose" }
    { "nounours_vert" }
}

$surfaceprop "carpet"
$keyvalues { "prop_data" { "base" "Cloth.Large" } }
$sequence "idle" "idle" loop fps 30.00
$collisionmodel "nounours" {

	$concave
	$mass 50.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}