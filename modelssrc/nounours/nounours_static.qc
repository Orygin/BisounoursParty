$cd "."
$modelname props_bisou/nounours_static.mdl
$model "Body" "nounours.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "nounours_rose" }
    { "nounours_vert" }
    { "nounours_pascouleur" }
    { "nounours_paille" }
}

$staticprop
$surfaceprop "carpet"

$sequence "idle" "nounours.smd" fps 30.00

$collisionmodel "nounours.smd"