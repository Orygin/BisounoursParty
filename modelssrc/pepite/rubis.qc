$scale 0.4
$cd "."
$modelname props_bisou/rubis.mdl
$model "Body" "rubis.smd"
$cdmaterials "models/props_bisou"

$staticprop
$surfaceprop "concrete"

$sequence idle "rubis" fps 30.00

$collisionmodel "rubis.smd"