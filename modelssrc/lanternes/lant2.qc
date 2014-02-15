$cd "."
$scale 0.3

$modelname props_bisou/lant2.mdl
$model "Body" "lant2.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "lanternes" }
    { "lanternes_eteintes" }
}

$staticprop
$surfaceprop "metal"

$sequence "idle" "lant2" fps 30.00

$collisionmodel "lant2.smd"