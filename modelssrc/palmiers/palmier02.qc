$scale 0.3
$cd "."
$modelname props_bisou/palmiers/palmier02.mdl
$model "Body" "palmier02.smd"
$cdmaterials "models/props_bisou/palmiers"

$staticprop
$surfaceprop "wood"

$sequence "idle" "palmier02" fps 30.00

$collisionmodel "palmier02_phy.smd" {
$concave
}