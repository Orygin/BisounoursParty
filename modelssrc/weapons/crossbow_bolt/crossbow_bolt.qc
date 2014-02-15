$cd "."
$modelname "crossbow_bolt.mdl"
$model "studio" "crossbow_bolt.smd"
$cdmaterials "models\weapons\"

$surfaceprop "default"
$hboxset "default"
$hbox 0 "cylinder"	     1.00   -1.50   -1.50    20.00    1.50    1.50

$attachment "petales" "cylinder" 0.50 0.00 0.00 rotate -0 -180 -180

$keyvalues
{
	particles
	{
		effect
		{
			name				"crossbow_flower_petales"
			attachment_type		follow_attachment
			attachment_point	petales
		}
	}
}

$sequence idle "crossbow_bolt" rotate -90 fps 30.00
$collisionmodel "crossbow_bolt.smd"
