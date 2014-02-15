$cd "."
$scale 0.12
$origin 0 -2 -10.2
$modelname "Weapons/W_missile.mdl"
$model "studio" "obus.smd"
$cdmaterials "models\items"

$hboxset "default"
$scale 1.00
$hbox 0 "cone"	    -5.00    5.00   -5.00     5.00   23.00    5.00
$scale 0.12

$attachment "0" "cone" -0.00 -16.00 0.00 rotate 0.00 90.00 0.00
$attachment "trainee" "cone" 0.00 40.00 0.00 rotate -0 -90 -0
$surfaceprop "metal"

$keyvalues
{
	particles
	{
		effect
		{
			name				"obus_trail"
			attachment_type		follow_attachment
			attachment_point	trainee
		}
	}
}

$sequence shell "obus_anim" autoplay loop fps 12.00 ACT_VM_IDLE 1
$collisionmodel "obus.smd"