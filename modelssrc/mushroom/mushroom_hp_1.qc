$cd "."
$scale 0.15
$origin 0 0 30
$modelname props_bisou/mushroom_hp_1.mdl
$model "Body" "mushroom_hp_1.smd"
$cdmaterials "models/props_bisou"
$texturegroup skinfamilies
{
    { "mushroomhd" }
    { "mushroomhd_pink" }
    { "mushroomhd_green" }
}

$staticprop
$surfaceprop "watermelon"

$sequence "idle" "mushroom_hp_1" fps 30.00

$collisionmodel "mushroom_hp_1.smd"
