$cd "."
$scale 0.55

$modelname props_bisou/lant1_big.mdl
$model "Body" "lant1.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "lanternes" }
    { "lanternes_eteintes" }
}

$staticprop
$surfaceprop "metal"

$sequence "idle" "lant1" fps 30.00

$collisionmodel "lant1.smd"