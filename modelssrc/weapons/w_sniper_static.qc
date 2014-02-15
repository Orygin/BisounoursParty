$cd "sniper\w_"
$modelname weapons/w_sniper_static.mdl
$model "studio" "shotgun_reference.smd"
$cdmaterials "models\weapons\w_shotgun\"

$staticprop
$surfaceprop "weapon"

$sequence "idle" "shotgun_reference.smd" fps 30.00

$collisionmodel "shotgun_reference.smd"
