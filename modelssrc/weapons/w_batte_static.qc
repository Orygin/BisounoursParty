$cd "batte\w_"
$modelname "weapons/w_batte_static.mdl"
$model "studio" "w_Crowbar_reference.smd"
$cdmaterials "models\weapons\w_batte\"

$staticprop
$surfaceprop "Wood_Solid"

$sequence idle "w_Crowbar_reference.smd" fps 30.00

$collisionmodel "w_Crowbar_reference.smd"