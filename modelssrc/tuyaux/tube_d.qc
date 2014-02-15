$cd "."
$scale 0.75

$modelname props_bisou/toyo/tube_d.mdl
$model "Body" "tube_d.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "tube_d" fps 30.00

$collisionmodel "tube_d.smd"