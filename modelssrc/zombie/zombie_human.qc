$cd "."
$modelname "Zombie/Bisounours.mdl"
$body studio "Soldier_reference.smd"

$lod 8
{
  replacemodel "Soldier_reference" "lod1_Soldier_reference"
}

$lod 20
{
  replacemodel "Soldier_reference" "lod1_Soldier_reference"
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
  replacemodel "Soldier_reference" "lod1_Soldier_reference"
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
    { "rose" }
    { "vert" }
    { "rouge" }
    { "orange" }
    { "bleu" }
    { "violet" }
    { "jaune" }
}

$poseparameter move_yaw -180.00 180.00 360.00
$cdmaterials "models\Zombie_Bisounours\"
$hboxset "default"
$hbox 2 "ValveBiped.Bip01_Spine"	    -1.00   -7.20   -9.00     7.00   10.80    9.00
$hbox 1 "ValveBiped.Bip01_Head1"	    -2.00   -4.00   -8.50    14.00   10.00    8.50
$hbox 4 "ValveBiped.Bip01_L_UpperArm"	-2.00   -2.75   -2.75    12.00    2.75    2.75
$hbox 4 "ValveBiped.Bip01_L_Forearm"	0.00   -2.25   -2.25    10.00    2.25    2.25
$hbox 4 "ValveBiped.Bip01_L_Hand"	    0.50   -1.00   -2.00     5.50    1.00    2.00
$hbox 5 "ValveBiped.Bip01_R_UpperArm"	-2.00   -2.75   -2.75    12.00    2.75    2.75
$hbox 5 "ValveBiped.Bip01_R_Forearm"	0.00   -2.25   -2.25    10.00    2.25    2.25
$hbox 5 "ValveBiped.Bip01_R_Hand"	    0.50   -1.00   -2.00     5.50    1.00    2.00
$hbox 6 "ValveBiped.Bip01_L_Thigh"	    1.00   -2.00   -3.50    19.00    8.00    6.50
$hbox 6 "ValveBiped.Bip01_L_Calf"	    0.50   -2.00   -2.60    15.50    6.00    6.40
$hbox 6 "ValveBiped.Bip01_L_Foot"	    -2.00   -1.50   -2.00     6.00    4.50    6.00
$hbox 7 "ValveBiped.Bip01_R_Thigh"	    1.00   -2.00   -6.50    19.00    8.00    3.50
$hbox 7 "ValveBiped.Bip01_R_Calf"	    0.50   -2.00   -6.40    15.50    6.00    2.60
$hbox 7 "ValveBiped.Bip01_R_Foot"	    -2.00   -1.50   -6.00     6.00    4.50    2.00
$hbox 0 "ValveBiped.Bip01_Pelvis"	    -11.50   -4.00  -11.00    11.50    4.00    7.00
$hbox 0 "ValveBiped.Bip01_Spine2"	    -1.00   -6.50   -7.50    13.00    6.50    7.50

$attachment "eyes" "ValveBiped.HC_Body_Bone" 0.00 0.00 0.00 rotate -0.00 0.00 -50.00
$attachment "head" "ValveBiped.HC_Body_Bone" 0.00 0.00 0.00 rotate -0.00 0.00 -50.00
$attachment "chest" "ValveBiped.Bip01_Spine4" -3.00 6.00 0.00 rotate 0.00 60.00 90.00
$attachment "maw" "ValveBiped.Bip01_Spine2" -0.00 8.00 0.00 rotate 0.00 90.00 90.00
$attachment "Blood_Left" "ValveBiped.Bip01_L_Finger2" -0.00 -0.00 0.00 rotate -0.00 -0.00 0.00
$attachment "Blood_Right" "ValveBiped.Bip01_R_Finger2" -0.00 -0.00 0.00 rotate 0.00 -0.00 0.00

$eyeposition 0.000 0.000 70.000

$contents "monster"
$surfaceprop "zombieflesh_bisou"
$jointsurfaceprop "valvebiped.bip01_head1" "zombieflesh_bisou_head"
$jointsurfaceprop "valvebiped.bip01_spine2" "zombieflesh_bisou_head"

$sequence Idle01 "Idle01" ACT_IDLE 1 fps 30.00

$sequence exhumation "slumprise_b" ACT_ZOMBIESPAWN 1 fps 30.00 {
  { event AE_ZOMBIE_STEP_LEFT 13 }
  { event AE_ZOMBIE_STEP_RIGHT 27 }
  { event AE_ZOMBIE_SCUFF_LEFT 45 }
  { event AE_ZOMBIE_SCUFF_RIGHT 60 }
}

$animation a_WalkS "a_WalkS" loop fps 30.000000 LX LY
$animation a_WalkSE "a_WalkSE" loop fps 30.000000 LX LY
$animation a_WalkE "a_WalkE" loop fps 30.000000 LX LY
$animation a_WalkNE "a_WalkNE" loop fps 30.000000 LX LY
$animation a_walk1 "a_walk1" loop fps 30.000000 LX LY
$animation a_WalkNW "a_WalkNW" loop fps 30.000000 LX LY
$animation a_WalkW "a_WalkW" loop fps 30.000000 LX LY
$animation a_WalkSW "a_WalkSW" loop fps 30.000000 LX LY

$sequence walk "a_WalkS" loop ACT_WALK 1 fps 30.00 {
  { event AE_ZOMBIE_STEP_LEFT 5 }
  { event AE_ZOMBIE_STEP_RIGHT 15 }
  { event AE_ZOMBIE_SCUFF_LEFT 20 }
  { event AE_ZOMBIE_STEP_LEFT 28 }
  { event AE_ZOMBIE_STEP_RIGHT 38 }
  { event AE_ZOMBIE_SCUFF_LEFT 40 }
  blendwidth 9
  blend move_yaw -180.000000 180.000000
 a_WalkSE a_WalkE a_WalkNE a_walk1 a_WalkNW a_WalkW a_WalkSW a_WalkS
}
$animation a_walk2 "a_walk2" loop fps 30.000000 LX LY
$sequence walk2 "a_WalkS" loop ACT_WALK 1 fps 30.00 {
  { event AE_ZOMBIE_STEP_LEFT 7 }
  { event AE_ZOMBIE_SCUFF_RIGHT 10 }
  { event AE_ZOMBIE_STEP_RIGHT 21 }
  { event AE_ZOMBIE_STEP_LEFT 31 }
  { event AE_ZOMBIE_SCUFF_RIGHT 34 }
  { event AE_ZOMBIE_STEP_RIGHT 44 }
  blendwidth 9
  blend move_yaw -180.000000 180.000000
 a_WalkSE a_WalkE a_WalkNE a_walk2 a_WalkNW a_WalkW a_WalkSW a_WalkS
}
$animation a_walk3 "a_walk3" loop fps 30.000000 LX LY
$sequence walk3 "a_WalkS" loop ACT_WALK 1 fps 30.00 {
  { event AE_ZOMBIE_STEP_LEFT 7 }
  { event AE_ZOMBIE_STEP_RIGHT 18 }
  { event AE_ZOMBIE_SCUFF_LEFT 21 }
  { event AE_ZOMBIE_STEP_LEFT 32 }
  { event AE_ZOMBIE_STEP_RIGHT 40 }
  { event AE_ZOMBIE_SCUFF_LEFT 44 }
  blendwidth 9
  blend move_yaw -180.000000 180.000000
 a_WalkSE a_WalkE a_WalkNE a_walk3 a_WalkNW a_WalkW a_WalkSW a_WalkS
}
$animation a_Walk4 "a_Walk4" loop fps 30.000000 LX LY
$sequence walk4 "a_Walk4" loop ACT_WALK 1 fps 30.00 {
  { event AE_ZOMBIE_STEP_LEFT 4 }
  { event AE_ZOMBIE_STEP_RIGHT 16 }
  { event AE_ZOMBIE_STEP_LEFT 22 }
  { event AE_ZOMBIE_STEP_RIGHT 28 }
  { event AE_ZOMBIE_STEP_LEFT 43 }
  { event AE_ZOMBIE_STEP_RIGHT 49 }
  { event AE_ZOMBIE_STEP_LEFT 58 }
  { event AE_ZOMBIE_STEP_RIGHT 67 }
}

$sequence FireWalk "FireWalk" loop ACT_WALK_ON_FIRE 1 fps 40.00 LX LY
$sequence FireIdle "FireIdle" loop ACT_IDLE_ON_FIRE 1 fps 30.00



$sequence attackA "bisou_attackA" ACT_MELEE_ATTACK1 2 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_RIGHT 2  }
}

$sequence attackB "bisou_attackB" ACT_MELEE_ATTACK1 2 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_RIGHT 2  }
}

$sequence attackC "bisou_attackC" ACT_MELEE_ATTACK1 2 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_LEFT 2  }
}

$sequence attackD "bisou_attackD" ACT_MELEE_ATTACK1 2 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_LEFT 2  }
}

$sequence attackE "bisou_attackE" ACT_MELEE_ATTACK1 1 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_BOTH 4 }
}

$sequence attackF "bisou_attackF" ACT_MELEE_ATTACK1 1 fps 30.00 {
  { event AE_ZOMBIE_ATTACK_SCREAM 1 }
  { event AE_ZOMBIE_ATTACK_BOTH 4 }
}



$sequence swatleftmid "swatleftmid" ACT_ZOM_SWATLEFTMID 1 fps 30.00 {
  { event AE_ZOMBIE_STARTSWAT 1 }
  { event AE_ZOMBIE_SWATITEM 9 }
}

$sequence swatrightmid "swatrightmid" ACT_ZOM_SWATRIGHTMID 1 fps 30.00 {
  { event AE_ZOMBIE_STARTSWAT 1 }
  { event AE_ZOMBIE_SWATITEM 9 }
}

$sequence swatleftlow "swatleftlow" ACT_ZOM_SWATLEFTLOW 1 fps 30.00 {
  { event AE_ZOMBIE_STARTSWAT 1 }
  { event AE_ZOMBIE_SWATITEM 15 }
}

$sequence swatrightlow "swatrightlow" ACT_ZOM_SWATRIGHTLOW 1 fps 30.00 {
  { event AE_ZOMBIE_STARTSWAT 1 }
  { event AE_ZOMBIE_SWATITEM 18 }
}

/*
$sequence releasecrab "releasecrab" ACT_ZOM_RELEASECRAB 1 fps 30.00
$sequence physflinch1 "physflinch1" ACT_FLINCH_PHYSICS 1 fps 30.00
$sequence physflinch2 "physflinch2" ACT_FLINCH_PHYSICS 1 fps 30.00
$sequence physflinch3 "physflinch3" ACT_FLINCH_PHYSICS 1 fps 30.00
$sequence smashfall64 "smashfall64" fps 30.00 {
  { event AE_NPC_BODYDROP_HEAVY 10 }
  { event AE_NPC_BODYDROP_HEAVY 27 }
  { event AE_NPC_BODYDROP_HEAVY 37 }
  { event AE_ZOMBIE_GET_UP 75 }
}

$sequence slump_a "slump_a" loop fps 30.00
$sequence slumprise_a "slumprise_a" fps 30.00 {
  { event AE_NPC_LEFTFOOT 15 }
  { event AE_NPC_RIGHTFOOT 50 }
  { event AE_ZOMBIE_GET_UP 80 }
}

$sequence slumprise_a_attack "slumprise_a_attack" fps 30.00 {
  { event AE_NPC_LEFTFOOT 15 }
  { event AE_NPC_RIGHTFOOT 50 }
  { event AE_ZOMBIE_GET_UP 80 }
  { event AE_ZOMBIE_ATTACK_SCREAM 16 }
  { event AE_ZOMBIE_ATTACK_RIGHT 36 }
}

$sequence slumprise_a2 "slumprise_a2" fps 30.00 {
  { event AE_NPC_RIGHTFOOT 16 }
  { event AE_NPC_RIGHTFOOT 22 }
  { event AE_ZOMBIE_GET_UP 24 }
}

$sequence slump_b "slump_b" loop fps 30.00
$sequence slumprise_b "slumprise_b" fps 30.00 {
  { event AE_NPC_LEFTFOOT 13 }
  { event AE_NPC_RIGHTFOOT 27 }
  { event AE_ZOMBIE_GET_UP 57 }
}

$sequence Breakthrough "Breakthrough" fps 30.00
$sequence canal5await "canal5await" fps 30.00
$sequence canal5aattack "canal5aattack" fps 30.00
$animation mdldecompiler_delta.smd "mdldecompiler_delta.smd" fps 30
$sequence flinch1inDelta "flinch1inDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch1inFrame "flinch1inFrame" fps 30.00
$sequence flinch1CoreDelta "flinch1CoreDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch1OutFrame "flinch1OutFrame" fps 30.00
$sequence flinch1outDelta "flinch1outDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch1 "flinch1" ACT_GESTURE_FLINCH_HEAD 1 fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch2inDelta "flinch2inDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch2inFrame "flinch2inFrame" fps 30.00
$sequence flinch2CoreDelta "flinch2CoreDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch2OutFrame "flinch2OutFrame" fps 30.00
$sequence flinch2outDelta "flinch2outDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch2 "flinch2" ACT_GESTURE_FLINCH_HEAD 1 fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch3inDelta "flinch3inDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch3inFrame "flinch3inFrame" fps 30.00
$sequence flinch3CoreDelta "flinch3CoreDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch3OutFrame "flinch3OutFrame" fps 30.00
$sequence flinch3outDelta "flinch3outDelta" fps 30.00 subtract mdldecompiler_delta.smd 0
$sequence flinch3 "flinch3" ACT_GESTURE_FLINCH_HEAD 1 fps 30.00 subtract mdldecompiler_delta.smd 0
*/

$sequence Tantrum "Tantrum" loop ACT_ZOMBIE_TANTRUM 1 fps 30.00
$sequence WallPound "WallPound" loop ACT_ZOMBIE_WALLPOUND 1 fps 30.00 {
  { event AE_ZOMBIE_POUND 14 }
  { event AE_ZOMBIE_POUND 35 }
}

$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00

$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.640 0.769 0.000
$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.607 0.795 0.000
$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.545 -0.838 0.000
$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.518 -0.855 -0.000

$collisionjoints "phymodel_human.smd" {

	$mass 100.0
	$inertia 2.00
	$damping 0.01
	$rotdamping 0.40
	$rootbone "valvebiped.bip01_pelvis"
	$jointmerge "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01_Spine1"

	$jointconstrain "valvebiped.bip01_spine2" x limit -48.00 48.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" y limit -25.00 25.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" z limit -25.00 50.00 0.20

	$jointconstrain "valvebiped.bip01_r_upperarm" x limit -50.00 57.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" y limit -19.00 102.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" z limit -93.00 30.00 0.20

	$jointconstrain "valvebiped.bip01_l_upperarm" x limit -50.00 57.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" y limit -102.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" z limit -93.00 30.00 0.20

	$jointmassbias "valvebiped.bip01_l_forearm" 2.00
	$jointconstrain "valvebiped.bip01_l_forearm" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" z limit -120.00 4.00 0.20

	$jointmassbias "valvebiped.bip01_l_hand" 4.00
	$jointconstrain "valvebiped.bip01_l_hand" x limit -120.00 120.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" z limit -57.00 70.00 0.20

	$jointmassbias "valvebiped.bip01_r_forearm" 2.00
	$jointconstrain "valvebiped.bip01_r_forearm" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" z limit -120.00 4.00 0.20

	$jointmassbias "valvebiped.bip01_r_hand" 4.00
	$jointconstrain "valvebiped.bip01_r_hand" x limit -120.00 120.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_hand" z limit -57.00 70.00 0.20

	$jointconstrain "valvebiped.bip01_r_thigh" x limit -25.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" y limit -8.00 75.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" z limit -97.00 32.00 0.20

	$jointmassbias "valvebiped.bip01_r_calf" 2.00
	$jointconstrain "valvebiped.bip01_r_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" z limit -12.00 126.00 0.20

	$jointmassbias "valvebiped.bip01_head1" 2.00
	$jointconstrain "valvebiped.bip01_head1" x limit -20.00 20.00 0.20
	$jointconstrain "valvebiped.bip01_head1" y limit -25.00 25.00 0.20
	$jointconstrain "valvebiped.bip01_head1" z limit -13.00 30.00 0.20

	$jointconstrain "valvebiped.bip01_l_thigh" x limit -25.00 20.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" y limit -73.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" z limit -93.00 30.00 0.20

	$jointmassbias "valvebiped.bip01_l_calf" 2.00
	$jointconstrain "valvebiped.bip01_l_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" z limit -8.00 126.00 0.20

	$jointmassbias "valvebiped.bip01_l_foot" 4.00
	$jointconstrain "valvebiped.bip01_l_foot" x limit -1.00 -1.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" y limit -19.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" z limit -15.00 35.00 0.20

	$jointmassbias "valvebiped.bip01_r_foot" 4.00
	$jointconstrain "valvebiped.bip01_r_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_foot" y limit -21.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_r_foot" z limit -15.00 35.00 0.20
	$animatedfriction 1.000 1000.000 0.800 0.500 0.000
}
