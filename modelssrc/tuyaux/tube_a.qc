$cd "."
$scale 0.75

$modelname props_bisou/toyo/tube_a.mdl
$model "Body" "tube_a.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "tube_a" fps 30.00

$collisionmodel "tube_a.smd"