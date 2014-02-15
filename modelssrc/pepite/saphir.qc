$scale 0.2
$cd "."
$modelname props_bisou/saphir.mdl
$model "Body" "saphir.smd"
$cdmaterials "models/props_bisou"

$staticprop
$surfaceprop "concrete"

$sequence idle "saphir" fps 30.00

$collisionmodel "saphir.smd"