$cd "."
$scale 0.55

$modelname props_bisou/lant3_big.mdl
$model "Body" "lant3.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "lanternes" }
    { "lanternes_eteintes" }
}

$staticprop
$surfaceprop "metal"

$sequence "idle" "lant3" fps 30.00

$collisionmodel "lant3.smd"