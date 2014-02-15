$cd "."
$scale 0.6
$modelname props_bisou/plant.mdl
$model "Body" "plant.smd"
$cdmaterials "models/props_bisou/plant"

$staticprop
$surfaceprop "foliage"
$mostlyopaque
$contents "notsolid"

$sequence "idle" "plant" fps 30.00
