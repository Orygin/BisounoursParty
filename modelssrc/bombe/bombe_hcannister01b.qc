$scale 4.50
$cd "."
$origin 0 -40 -45

$modelname "props_combine/headcrabcannister01b.mdl"
$model "body" "bombe.smd"

$surfaceprop "canister"

$cdmaterials "models/props_bisou"
$sequence "idle_open" "bombe_hcannister" loop fps 30.00
$sequence "idle_closed" "bombe_hcannister" loop fps 30.00
$sequence "open" "bombe_hcannister" fps 30.00

$collisionmodel "bombe_physbox.smd" {
	$concave
}