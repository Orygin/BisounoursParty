$cd "."
$modelname props_bisou/lolwut.mdl
$model "Body" "lolwut_ref.smd"
$cdmaterials "models/props_bisou"

$lod 200
{
    replacemodel "lolwut_ref" "lolwut_lod"
}

$cdmaterials "models/props_bisou"
$staticprop
$surfaceprop "watermelon"

$sequence "idle" "lolwut_ref" fps 30.00

$collisionmodel "lolwut_ref.smd"
