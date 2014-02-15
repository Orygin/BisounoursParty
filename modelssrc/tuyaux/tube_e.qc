$cd "."
$scale 0.75

$modelname props_bisou/toyo/tube_e.mdl
$model "Body" "tube_e.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "tube_e" fps 30.00

$collisionmodel "tube_e.smd"