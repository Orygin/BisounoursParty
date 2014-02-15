$cd "."
$modelname "props_bisou\glace.mdl"
$model "Body" "glace.smd"
$cdmaterials "models\props_bisou\"
$texturegroup skinfamilies
{
	{ "glace_verte" }
	{ "glace_rose" }
}
$staticprop
$surfaceprop "dirt"
$sequence idle "glace" loop fps 1
$collisionmodel "glace_phys.smd"
