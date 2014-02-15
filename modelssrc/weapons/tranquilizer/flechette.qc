$scale 0.4
$origin -4.5 0 0

$cd "."
$modelname Weapons/flechette
$model "studio" "Flechette.smd"
$cdmaterials "models/weapons/flechette"

$texturegroup skinfamilies
{
	{ "plume_rouge"
	 "metal_rouge" }
	{ "plume_rose"
	 "metal_rose" }
	{ "plume_vert"
	 "metal_vert" }
}

$surfaceprop "default"
$hboxset "default"
$scale 1.00
$hbox 0 "Cylindre"	    -3.00   -2.50   -2.50     7.00    2.50    2.50
$scale 0.08

$sequence "idle" "idle" rotate -90 fps 30.00
$collisionmodel "Flechette.smd"