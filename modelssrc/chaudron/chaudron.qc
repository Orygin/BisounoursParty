$cd "."
$modelname "props_bisou/chaudron.mdl"
$model "Body" "chaudron_ref.smd" 
$cdmaterials "models/props_bisou"

$staticprop 
$surfaceprop "metal"

$sequence Idle "chaudron_idle" fps 1

$collisionmodel "chaudron_phys.smd" 
{ 
$concave 
}
