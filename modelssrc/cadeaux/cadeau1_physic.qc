$scale 0.16
$cd "."
$modelname props_bisou/cadeau1_physic.mdl
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

$surfaceprop "cardboard"
$keyvalues  { prop_data { "base" "Cardboard.Medium" "health" "0"  } } 

$sequence "idle" "cadeau1" fps 30.00

$collisionmodel "cadeau1_phy.smd"