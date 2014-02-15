$cd "."
$origin 0 0 0
$scale 2.10

$modelname "props_bisou/bloc_choco_2x2.mdl"
$model "body" "choco_2_2.smd"

$surfaceprop "chocolat"
$keyvalues {
   "prop_data" {
      "base" "Plastic.Medium"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}

$lod 90
{
	replacematerial "choco"  				"chocolod1"
	replacemodel 	"choco_2_2" 			"choco_2_2_lod"
}
$lod 120
{
	replacematerial "choco"  				"chocolod2"
	replacemodel 	"choco_2_2" 			"choco_2_2_lod"
}

$cdmaterials "models/props_bisou"
$sequence "idle" "choco_2_2" loop fps 30.00

$collisionmodel "choco_2_2.smd" {
	$mass 80.0
	$concave
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}