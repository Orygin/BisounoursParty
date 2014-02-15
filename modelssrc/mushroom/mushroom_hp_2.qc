$cd "."
$scale 0.50
$origin 0 0 17
$modelname props_bisou/mushroom_hp_2.mdl
$model "Body" "mushroom_hp_2.smd"
$cdmaterials "models/props_bisou"
$texturegroup skinfamilies
{
    { "mushroomhd" }
    { "mushroomhd_pink" }
    { "mushroomhd_green" }
}

$staticprop
$surfaceprop "watermelon"

$sequence "idle" "mushroom_hp_2" fps 30.00

$collisionmodel "mushroom_hp_2.smd"
