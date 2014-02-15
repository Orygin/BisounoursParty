$cd "."
$scale 0.90

$modelname "props_bisou/bloc_lego1.mdl"
$model "body" "lego.smd"

$surfaceprop "lego"
$keyvalues {
   "prop_data" {
      "base" "Plastic.Medium"
	  "dmg.bullets"		0
	  "dmg.club"		0
	  "dmg.explosive"	0
   }
}

$texturegroup skinfamilies
{
    { "lego" }
    { "lego1" }
    { "lego2" }
    { "lego3" }
}

$lod 30
{
  replacemodel "lego" "lego_lod1"
}
$lod 70
{
  replacemodel "lego" "lego_lod2"
}
$lod 90
{
	replacematerial "lego"  				"lego_lod"
	replacematerial "lego1"  				"lego1_lod"
	replacematerial "lego2"  				"lego2_lod"
	replacematerial "lego3"  				"lego3_lod"
	replacemodel 	"lego" 					"lego_lod2"
}
$lod 120
{
  replacemodel "lego" "lego_phys"
}

$cdmaterials "models/props_bisou"
$sequence "idle" "lego" loop fps 30.00

$collisionmodel "lego_phys.smd" {
	$mass 120.0
	$concave
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}