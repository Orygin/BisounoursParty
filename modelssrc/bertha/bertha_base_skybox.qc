$scale 0.20
$cd "."
$modelname props_bisou/bertha_base_skybox.mdl
$model "Body" "bertha_base.smd"

$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "bertha_base" }
    { "bertha_base2" }
}

$surfaceprop "metal"

$sequence "Idle01" "bertha_base" loop ACT_VM_IDLE 1 fps 1.50
$collisionmodel "bertha_base.smd"