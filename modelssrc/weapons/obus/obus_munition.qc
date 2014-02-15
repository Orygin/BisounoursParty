$cd "."
$scale 0.12
$origin 0 0 -11
$modelname "Weapons/W_missile_closed.mdl"
$model "studio" "obus_munition.smd"
$cdmaterials "models\items"

$surfaceprop "metal"
$illumposition -0.005 0.110 0.220
$sequence shell "obus_munition" rotate 0.00 fps 30.00
$keyvalues {
   "prop_data" {
      "base" "Metal.Medium"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}
$collisionmodel "obus_munition.smd" {

	$concave
	$mass 100.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
