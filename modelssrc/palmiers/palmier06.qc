$scale 0.3
$cd "."
$modelname props_bisou/palmiers/palmier06.mdl
$model "Body" "palmier06.smd"
$cdmaterials "models/props_bisou/palmiers"

$staticprop
$surfaceprop "wood"

$sequence "idle" "palmier06" fps 30.00

$collisionmodel "palmier06_phy.smd" {
$concave
}