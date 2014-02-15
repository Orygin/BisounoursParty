$cd "."
$scale 0.20
$modelname props_bisou/cactus1.mdl
$model "Body" "cactus1.smd"
$cdmaterials "models/props_bisou"

$lod 80
{
    replacemodel "cactus1" "cactus1_lod1"
}

$staticprop
$surfaceprop "watermelon"

$sequence "idle" "cactus1" fps 30.00

$collisionmodel "cactus1_lod1.smd"
