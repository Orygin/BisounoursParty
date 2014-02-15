$scale 1.0
$cd "."
$modelname props_bisou/bertha_canon.mdl
$model "Body" "bertha_canon.smd"

$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "bertha_canon" }
    { "bertha_canon2" }
}

$surfaceprop "metal"

$sequence "Idle01" "bertha_canon" loop ACT_VM_IDLE 1 fps 1.50
$collisionmodel "bertha_canon.smd"