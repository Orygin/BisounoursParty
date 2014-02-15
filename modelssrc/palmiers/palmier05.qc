$scale 0.3
$cd "."
$modelname props_bisou/palmiers/palmier05.mdl
$model "Body" "palmier05.smd"
$cdmaterials "models/props_bisou/palmiers"

$staticprop
$surfaceprop "wood"

$sequence "idle" "palmier05" fps 30.00

$collisionmodel "palmier05_phy.smd" {
$concave
}