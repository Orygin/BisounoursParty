$scale 1.0
$cd "."
$modelname props_bisou/pepite.mdl
$model "Body" "pepite.smd"
$cdmaterials "models/props_bisou"

$staticprop
$surfaceprop "concrete"

$sequence idle "pepite" fps 30.00

$collisionmodel "pepite.smd"