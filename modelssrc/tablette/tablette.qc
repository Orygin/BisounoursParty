$cd "."
$scale 0.15
$origin 0 11 -1.7
$modelname "items/battery.mdl"
$model "body" "tablette_ref.smd"
$cdmaterials "models/items"
$surfaceprop "chocolat"
$keyvalues {
   "prop_data" {
      "base" "Plastic.Small"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}
$sequence "idle" "tablette_ref" loop fps 30.00
$collisionmodel "tablette_ref" {

	$concave
	$mass 15.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}