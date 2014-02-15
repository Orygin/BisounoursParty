$cd "."
$scale 0.75

$modelname props_bisou/toyo/tube_b.mdl
$model "Body" "tube_b.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "tube_b" fps 30.00

$collisionmodel "tube_b.smd"