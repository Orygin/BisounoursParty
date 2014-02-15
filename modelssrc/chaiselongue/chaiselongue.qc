$cd "."
$scale 1.00

$modelname "props_bisou/chaiselongue.mdl"
$model "body" "chaiselongue.smd"

$surfaceprop "bambou"
$keyvalues {
   "prop_data" {
      "base" "Wooden.Large"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}

$cdmaterials "models/props_bisou"
$sequence "idle" "chaiselongue" loop fps 30.00

$collisionmodel "chaiselongue.smd" {
	$mass 30.0
	$concave
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}