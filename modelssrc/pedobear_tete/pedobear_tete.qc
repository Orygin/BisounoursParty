$cd "."
$scale 1.3
$modelname props_bisou/pedobear_tete.mdl
$model "Body" "pedobear_tete.smd"
$cdmaterials "models/props_bisou"

$cdmaterials "models/props_bisou"
$staticprop
$surfaceprop "pedobear"

$sequence "idle" "pedobear_tete" fps 30.00

$collisionmodel "pedobear_tete.smd"
