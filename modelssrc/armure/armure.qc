$scale 0.3
$cd "."
$modelname items/armure.mdl
$model "Body" "armure_ref.smd"
$cdmaterials "models/items"
$origin 0 0 -20

$texturegroup skinfamilies
{
    { "armure" }
    { "armure1" }
    { "armure2" }
    { "armure3" }
}

//$staticprop
$surfaceprop "chocolat"

$sequence "Idle01" "armure_anim" loop ACT_VM_IDLE 1 fps 1.50
$sequence "Idle02" "armure_ref" loop fps 30.00

//$collisionmodel "physbox.smd"