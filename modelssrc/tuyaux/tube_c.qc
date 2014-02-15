$cd "."
$scale 0.75

$modelname props_bisou/toyo/tube_c.mdl
$model "Body" "tube_c.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "tube_c" fps 30.00

$collisionmodel "tube_c.smd"