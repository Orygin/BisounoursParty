$cd "bigshotgun\w_"
$modelname weapons/w_bigshotgun_static.mdl
$model "studio" "shotgun_reference.smd"
$cdmaterials "models\weapons\v_bigshotgun\"

$staticprop
$surfaceprop "weapon"

$sequence "idle" "shotgun_reference.smd" fps 30.00

$collisionmodel "shotgun_reference.smd"
