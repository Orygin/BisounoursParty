$cd "."
$origin 0 -2.5 -2.8
$scale 0.28

$modelname "props_combine/headcrabcannister01a_skybox.mdl"
$model "body" "bombe.smd"

$surfaceprop "canister"

$cdmaterials "models/props_bisou"
$sequence "idle" "bombe_hcannister" loop fps 30.00

//$collisionmodel "bombe_physbox.smd" {
//	$concave
//}