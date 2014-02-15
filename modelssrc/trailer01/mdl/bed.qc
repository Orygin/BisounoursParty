//$scale 0.3
$cd "."
$modelname props_bisou/lit.mdl
$model "Body" "bed.smd"
$collisionmodel "bed.smd"
$cdmaterials "models/props_bisou/"

$staticprop
$surfaceprop "wood"

$sequence "idle" "bed_idle" fps 30.00
