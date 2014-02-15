$scale 0.5
$cd "."
$modelname props_bisou/cadeau1_static.mdl
$model "Body" "cadeau1.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "cadeau1" }
    { "cadeau2" }
    { "cadeau3" }
    { "cadeau4" }
    { "cadeau5" }
}

$staticprop
$surfaceprop "cardboard"

$sequence "idle" "cadeau1" fps 30.00

$collisionmodel "cadeau1_phy.smd"