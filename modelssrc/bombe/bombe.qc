$cd "."
$scale 4.50

$modelname "props_bisou/bombe.mdl"
$model "body" "bombe.smd"

$keyvalues {
   "prop_data" {
      "base" "metal.large"
   }
}
$surfaceprop "canister"

$cdmaterials "models/props_bisou"
$sequence "idle" "bombe" loop fps 30.00

$collisionmodel "bombe_physbox.smd" {
	$mass 300.0
	$concave
}