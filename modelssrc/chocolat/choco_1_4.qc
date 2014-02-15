$cd "."
$origin -20 40 0
$scale 2.10

$modelname "props_bisou/bloc_choco_1x4.mdl"
$model "body" "choco_1_4.smd"

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
	replacemodel 	"choco_1_4" 			"choco_1_4_lod"
}
$lod 120
{
	replacematerial "choco"  				"chocolod2"
	replacemodel 	"choco_1_4" 			"choco_1_4_lod"
}

$cdmaterials "models/props_bisou"
$sequence "idle" "choco_1_4" loop fps 30.00

$collisionmodel "choco_1_4.smd" {
	$mass 80.0
	$concave
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}