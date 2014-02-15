$cd "slam\w_"
$origin 0 0 1.7
$modelname "weapons/w_slam.mdl"
$model "slam" "w_slam_reference.smd"

$hboxset "default"

$cdmaterials "models\Weapons\V_hand\"
$cdmaterials "models\Weapons\w_slam\"

$texturegroup skinfamilies
{
    { "mine_diode" 
	 "led1_on"
	 "led2_off" 
	 "led3_off" }

    { "led1_on" 
	 "led1_on"
	 "led2_off" 
	 "led3_off" }

    { "mine_diode" 
	 "led1_off"
	 "led2_on" 
	 "led3_off" }

    { "led2_on" 
	 "led1_off"
	 "led2_on" 
	 "led3_off" }

    { "mine_diode" 
	 "led1_off"
	 "led2_off" 
	 "led3_on" }

    { "led3_on" 
	 "led1_off"
	 "led2_off" 
	 "led3_on" }

    { "mine_diode" 
	 "led1_off"
	 "led2_off" 
	 "led3_off" }
}

$attachment "beam_attach" "SLAM" 0.00 2.60 0.00 rotate 0 0 0

$surfaceprop "weapon"
$illumposition -0.212 0.035 -0.629
$sequence idle "w_slam_reference" loop ACT_IDLE 1 fps 30.00 rotate 180
$sequence attack1 "w_slam_reference" loop ACT_RANGE_ATTACK 1 fps 30.00 rotate 180
$collisionmodel "w_slam_reference.smd" {
	$mass 1.0
	$inertia 1.00
	$damping 0.00
}

$jointsurfaceprop "ValveBiped.Bip01_R_Hand"	 "weapon"

$hbox 0 "SLAM"	    -8.00   -1.00   -8.00     8.00    5.00    8.00