$cd "."
$scale 0.6

$modelname "props_bisou/lampe1.mdl"
$model "studio" "lampe1_reference.smd"

$scale 0.75

$lod 60
{
	replacemodel 	"lampe1_reference" 	"lampe1_reference_lod1"
}

$scale 0.6

$staticprop
$cdmaterials "models\props_bisou\"

$texturegroup skinfamilies
{
    { "lampe1_allume" }
    { "lampe1_eteint" }
}

$surfaceprop "glass"
$sequence idle "lampe1_idle" ACT_VM_IDLE 1 fps 30.00
$collisionmodel "lampe1_reference.smd"