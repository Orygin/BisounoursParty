$cd "."
$scale 0.75

$modelname props_bisou/toyo/embout.mdl
$model "Body" "embout.smd"
$cdmaterials "models/props_bisou/toyo"

$staticprop
$surfaceprop "metal"

$sequence "idle" "embout" fps 30.00

$collisionmodel "embout.smd"