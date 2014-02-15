$scale 4.50
$cd "."
$origin 0 -40 -45

$modelname "props_combine/headcrabcannister01a.mdl"
$model "body" "bombe.smd"

$surfaceprop "canister"

$cdmaterials "models/props_bisou"
$sequence "idle" "bombe_hcannister" loop fps 30.00

$collisionmodel "bombe_physbox.smd" {
	$concave
}