$cd "."
$scale 0.15
$modelname "items/vodka.mdl"
$model "body" "vodka_ref.smd"
$cdmaterials "models/items"
$surfaceprop "glass"
$keyvalues {
   "prop_data" {
      "base" 			"Glass.Small"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}
$sequence "idle" "idle" loop fps 30.00
$collisionmodel "phymodel" {

	$concave
	$mass 25.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}