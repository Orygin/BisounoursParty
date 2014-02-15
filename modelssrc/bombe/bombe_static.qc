$cd "."
$scale 4.50
$modelname "props_bisou/bombe_static.mdl"
$model "body" "bombe.smd"

$cdmaterials "models/props_bisou"
$staticprop
$surfaceprop "canister"

$sequence "idle" "bombe" loop fps 30.00

$collisionmodel "bombe_physbox.smd"