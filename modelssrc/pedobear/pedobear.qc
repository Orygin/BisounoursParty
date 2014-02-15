$cd "."

$hboxset "default"
$hbox 4 "ValveBiped.Bip01_L_UpperArm"	    -5.50   -6.00   -3.50    19.50    4.00    3.50
$hbox 4 "ValveBiped.Bip01_L_Forearm"	    -3.00   -4.00   -2.50    17.00    2.00    2.50
$hbox 4 "ValveBiped.Bip01_L_Hand"	    	-3.00   -0.50   -5.50     9.00    4.50    3.50
$hbox 5 "ValveBiped.Bip01_R_UpperArm"	    -5.50   -6.00   -3.50    19.50    4.00    3.50
$hbox 5 "ValveBiped.Bip01_R_Forearm"	    -3.00   -4.00   -2.50    17.00    2.00    2.50
$hbox 5 "ValveBiped.Bip01_R_Hand"	    	-3.00   -0.50   -3.50     9.00    4.50    5.50
$hbox 6 "ValveBiped.Bip01_L_Thigh"	    	10.00   -5.50   -6.00    30.00    9.50    6.00
$hbox 6 "ValveBiped.Bip01_L_Calf"	     	5.00   -6.00   -8.00    25.00    8.00    4.00
$hbox 7 "ValveBiped.Bip01_R_Thigh"	    	10.00   -5.50   -6.00    30.00    9.50    6.00
$hbox 7 "ValveBiped.Bip01_R_Calf"	     	5.00   -6.00   -4.00    25.00    8.00    8.00
$hbox 2 "ValveBiped.Bip01_Spine2"	    	-5.00   -5.40  -15.00    15.00   15.40   15.00
$hbox 2 "ValveBiped.Bip01"	   				-14.00  -10.00   -8.50    14.00   10.00   10.50
$hbox 1 "ValveBiped.Bip01_Head1"	    	-3.50  -14.50  -14.50    21.50    8.50   14.50

$scale 1.3
$modelname "Zombie/poison.mdl"
$body studio "Zombiefast_reference.smd"

$lod 30
{
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$lod 60
{
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$shadowlod
{
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$texturegroup skinfamilies
{
    { "pedobear" }
    { "pedobear1" }
    { "pedobear2" }
    { "pedobear3" }
}

//$poseparameter move_yaw -180.00 180.00
$cdmaterials "models\pedobear\"

$attachment "headcrab" "ValveBiped.HC_BodyCube" -0.00 -1.50 0.00 rotate 0.00 0.00 70.00
$attachment "head" "ValveBiped.HC_BodyCube" -0.00 0.00 0.00 rotate 0.00 0.00 0.00
$attachment "chest" "ValveBiped.Bip01_Spine4" -4.00 0.00 3.00 rotate -70.00 0.00 -180.00
$attachment "Blood_Left" "ValveBiped.Bip01_L_Finger2" 0.00 -0.00 -0.00 rotate -0.00 0.00 0.00
$attachment "Blood_Right" "ValveBiped.Bip01_R_Finger2" -0.00 0.00 0.00 rotate -0.00 0.00 -0.00
$eyeposition 0.000 0.000 70.000

$contents "monster"
$surfaceprop "pedobear"
$jointsurfaceprop "valvebiped.bip01_head1" "pedobear_head"
$alwayscollapse "ValveBiped.Bip01_R_Finger2"
$alwayscollapse "ValveBiped.Bip01_L_Finger2"
$alwayscollapse "ValveBiped.HC_BodyCube"
$realignbones
$collapsebones

$sequence refpose "idle" fps 100.00

$sequence trailer "trailer-animation-pedobear" loop fps 30.00

$sequence releasecrab "idle" ACT_ZOM_RELEASECRAB 1 fps 100.00
$sequence headcrab2Leap "idle" ACT_RANGE_ATTACK1 1 fps 100.00 {
  { event AE_ZOMBIE_POISON_PICKUP_CRAB 1 }
  { event AE_ZOMBIE_POISON_THROW_CRAB 2 }
}
$sequence Throw "idle" ACT_RANGE_ATTACK2 1 fps 100.00 {
  { event AE_ZOMBIE_POISON_PICKUP_CRAB 1 }
  { event AE_ZOMBIE_POISON_THROW_CRAB 2 }
}

$sequence Idle01 "idle" loop ACT_IDLE 1 fps 30.00

$sequence Walk "Run" loop ACT_WALK 1 fps 35.00 LX LY {
  { event AE_ZOMBIE_STEP_LEFT 5 }
  { event AE_ZOMBIE_STEP_RIGHT 9 }
}

$sequence melee_01 "Melee" ACT_MELEE_ATTACK1 1 fps 25.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_RIGHT 3 }
  { event AE_ZOMBIE_ATTACK_LEFT 7 }
}

$sequence BR2_Attack01 "BR2_Attack" ACT_ZOM_SWATRIGHTLOW 1 fps 30.00 {
  { event AE_ZOMBIE_SWATITEM 18 }
}

$sequence BR2_Attack02 "BR2_Attack" ACT_ZOM_SWATLEFTLOW 1 fps 30.00 {
  { event AE_ZOMBIE_SWATITEM 18 }
}

$sequence Flinch "idle_angry" ACT_SMALL_FLINCH 1 fps 40.00

$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00



$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.774 -0.633 0.000
$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.000 0.000 0.000
$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.759 0.651 -0.000
$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.726 0.688 0.000
$collisionjoints "phymodel.smd" {

	$mass 95.0
	$inertia 2.00
	$damping 0.01
	$rotdamping 0.40
	$rootbone "valvebiped.bip01_pelvis"

	$jointconstrain "valvebiped.bip01_spine" x limit -32.00 32.00 0.20
	$jointconstrain "valvebiped.bip01_spine" y limit -39.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_spine" z limit -15.00 40.00 0.20

	$jointconstrain "valvebiped.bip01_spine2" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_spine2" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" z limit -41.00 41.00 0.20

	$jointconstrain "valvebiped.bip01_l_upperarm" x limit -30.00 61.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" y limit -90.00 48.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" z limit -106.00 68.00 0.20

	$jointmassbias "valvebiped.bip01_l_forearm" 2.00
	$jointconstrain "valvebiped.bip01_l_forearm" x limit 1.00 1.00 0.20
	$jointconstrain "valvebiped.bip01_l_forearm" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_forearm" z limit -102.00 44.00 0.20

	$jointmassbias "valvebiped.bip01_l_hand" 2.00
	$jointconstrain "valvebiped.bip01_l_hand" x limit -39.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" y limit -19.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" z limit -64.00 26.00 0.20

	$jointconstrain "valvebiped.bip01_r_upperarm" x limit -41.00 37.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" y limit -44.00 90.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" z limit -106.00 68.00 0.20

	$jointmassbias "valvebiped.bip01_r_forearm" 2.00
	$jointconstrain "valvebiped.bip01_r_forearm" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_forearm" z limit -41.00 93.00 0.20

	$jointconstrain "valvebiped.bip01_r_thigh" x limit -28.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" y limit -37.00 26.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" z limit -82.00 44.00 0.20

	$jointmassbias "valvebiped.bip01_r_calf" 2.00
	$jointconstrain "valvebiped.bip01_r_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" z limit -8.00 130.00 0.20

	$jointmassbias "valvebiped.bip01_r_foot" 2.00
	$jointconstrain "valvebiped.bip01_r_foot" x limit -30.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_r_foot" y limit -28.00 28.00 0.20
	$jointconstrain "valvebiped.bip01_r_foot" z limit -30.00 26.00 0.20

	$jointconstrain "valvebiped.bip01_l_thigh" x limit -37.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" y limit -30.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" z limit -75.00 39.00 0.20

	$jointmassbias "valvebiped.bip01_l_calf" 2.00
	$jointconstrain "valvebiped.bip01_l_calf" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_calf" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_calf" z limit -8.00 130.00 0.20

	$jointmassbias "valvebiped.bip01_l_foot" 2.00
	$jointconstrain "valvebiped.bip01_l_foot" x limit -10.00 10.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" y limit -17.00 17.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" z limit -21.00 39.00 0.20

	$jointmassbias "valvebiped.bip01_r_hand" 2.00
	$jointconstrain "valvebiped.bip01_r_hand" x limit -58.00 58.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" y limit -28.00 28.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" z limit -35.00 46.00 0.20

	$jointconstrain "valvebiped.bip01_Head1" x limit -30 30 0.20
	$jointconstrain "valvebiped.bip01_Head1" y limit -20 20 0.20
	$jointconstrain "valvebiped.bip01_Head1" z limit -20 20 0.20
}
