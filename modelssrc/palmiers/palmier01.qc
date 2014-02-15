$scale 0.3
$cd "."
$modelname props_bisou/palmiers/palmier01.mdl
$model "Body" "palmier01.smd"
$cdmaterials "models/props_bisou/palmiers"

$staticprop
$surfaceprop "wood"

$sequence "idle" "palmier01" fps 30.00

$collisionmodel "palmier01_phy.smd" {
$concave
}