$scale 0.3
$cd "."
$modelname props_bisou/palmiers/palmier03.mdl
$model "Body" "palmier03.smd"
$cdmaterials "models/props_bisou/palmiers"

$staticprop
$surfaceprop "wood"

$sequence "idle" "palmier03" fps 30.00

$collisionmodel "palmier03_phy.smd" {
$concave
}