$cd "."

$modelname "Zombie/Classic.mdl"
$body studio "Zombie_reference.smd"
$bodygroup head
{
	studio "Headcrab_reference.smd"
	blank
}

$lod 1
{
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$lod 8
{
  replacemodel "Zombie_reference" "lod1_Zombie_reference"
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$lod 20
{
  replacemodel "Zombie_reference" "lod1_Zombie_reference"
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
  replacemodel "Zombie_reference" "lod1_Zombie_reference"
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$poseparameter move_yaw -180.00 180.00 360.00
$cdmaterials "models\zombie_classic\"
$hboxset "default"
$hbox 2 "ValveBiped.Bip01_Spine1"	    -9.00    1.50   -7.00    15.00    4.50    7.00
$hbox 4 "ValveBiped.Bip01_L_UpperArm"	    -1.00   -2.50   -4.00    11.00    0.50    1.00
$hbox 4 "ValveBiped.Bip01_L_Forearm"	    -1.00   -2.50   -3.00    11.00    0.50    2.00
$hbox 4 "ValveBiped.Bip01_L_Hand"	    -1.00   -0.50   -2.50     7.00    2.50    3.50
$hbox 5 "ValveBiped.Bip01_R_UpperArm"	    -1.00   -2.50   -1.00    11.00    0.50    4.00
$hbox 5 "ValveBiped.Bip01_R_Forearm"	    -0.50   -2.50   -2.00    10.50    2.50    2.00
$hbox 5 "ValveBiped.Bip01_R_Hand"	    -1.00   -3.50   -1.50     7.00    3.50    1.50
$hbox 6 "ValveBiped.Bip01_L_Thigh"	    -1.00   -0.50   -4.00    17.00    2.50    4.00
$hbox 6 "ValveBiped.Bip01_L_Calf"	    -1.00   -0.50   -3.00    21.00    2.50    3.00
$hbox 7 "ValveBiped.Bip01_R_Thigh"	    -1.00   -0.50   -4.00    17.00    2.50    4.00
$hbox 7 "ValveBiped.Bip01_R_Calf"	    -1.00   -0.50   -3.00    21.00    2.50    3.00
$hbox 1 "ValveBiped.HC_Body_Bone"	    -4.50   -4.50   -3.50    -1.50    8.50    9.50

$attachment "eyes" "ValveBiped.HC_Body_Bone" 0.00 0.00 0.00 rotate -0.00 0.00 -50.00
$attachment "head" "ValveBiped.HC_Body_Bone" 0.00 0.00 0.00 rotate -0.00 0.00 -50.00
$attachment "chest" "ValveBiped.Bip01_Spine4" -3.00 6.00 0.00 rotate 0.00 60.00 90.00
$attachment "maw" "ValveBiped.Bip01_Spine2" -0.00 8.00 0.00 rotate 0.00 90.00 90.00
$attachment "Blood_Left" "ValveBiped.Bip01_L_Finger2" -0.00 -0.00 0.00 rotate -0.00 -0.00 0.00
$attachment "Blood_Right" "ValveBiped.Bip01_R_Finger2" -0.00 -0.00 0.00 rotate 0.00 -0.00 0.00
$eyeposition 0.000 0.000 70.000

$contents "monster"
$surfaceprop "painepice"
$jointsurfaceprop "BONEPD" "painepice_head"

$sequence Idle01 "Idle01" ACT_IDLE 1 fps 30.00

$sequence Teck01 "tecktonik1" fps 30.00 {
{ event 5004 2 "Tecktonik" }
}
$sequence Teck02 "tecktonik2" fps 30.00 {
{ event 5004 2 "Tecktonik" }
}


$animation RunS "RunS" loop fps 25.000000 LX LY
$animation RunSE "RunSE" loop fps 25.000000 LX LY
$animation RunE "RunE" loop fps 25.000000 LX LY
$animation RunNE "RunNE" loop fps 25.000000 LX LY
$animation RunN "RunN" loop fps 25.000000 LX LY
$animation RunNW "RunNW" loop fps 25.000000 LX LY
$animation RunW "RunW" loop fps 25.000000 LX LY
$animation RunSW "RunSW" loop fps 25.000000 LX LY

$sequence walk "RunS" loop ACT_WALK 1 fps 25.00 {
  { event AE_ZOMBIE_STEP_LEFT 5 }
  { event AE_ZOMBIE_STEP_RIGHT 15 }
  blendwidth 9
  blend move_yaw -180.000000 180.000000
 RunSE RunE RunNE RunN RunNW RunW RunSW RunS
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

$sequence Tantrum "Tantrum" loop ACT_ZOMBIE_TANTRUM 1 fps 50.00

$sequence WallPound "WallPound" loop ACT_ZOMBIE_WALLPOUND 1 fps 30.00 {
  { event AE_ZOMBIE_POUND 14 }
  { event AE_ZOMBIE_POUND 35 }
}

$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00

$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.640 0.769 0.000
$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.607 0.795 0.000
$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.545 -0.838 0.000
$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.518 -0.855 -0.000
$collisionjoints "phymodel.smd" {

	$mass 100.0
	$inertia 10.00
	$damping 0.10
	$rotdamping 3.00
	$rootbone "valvebiped.bip01_pelvis"
	$jointmerge "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01_Spine1"

	$jointconstrain "valvebiped.bip01_spine2" x limit -44.00 44.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" y limit -39.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" z limit -32.00 50.00 0.20

	$jointconstrain "valvebiped.bip01_r_clavicle" x limit -30.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_r_clavicle" y limit -46.00 46.00 0.20
	$jointconstrain "valvebiped.bip01_r_clavicle" z limit -26.00 48.00 0.20

	$jointconstrain "valvebiped.bip01_r_upperarm" x limit -39.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" y limit -79.00 95.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" z limit -93.00 28.00 0.20

	$jointconstrain "valvebiped.bip01_l_clavicle" x limit -30.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_l_clavicle" y limit -46.00 46.00 0.20
	$jointconstrain "valvebiped.bip01_l_clavicle" z limit -26.00 48.00 0.20

	$jointconstrain "valvebiped.bip01_l_upperarm" x limit -30.00 30.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" y limit -95.00 84.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" z limit -86.00 26.00 0.20

	$jointconstrain "valvebiped.bip01_l_forearm" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_forearm" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_forearm" z limit -149.00 4.00 0.20

	$jointconstrain "valvebiped.bip01_l_hand" x limit -55.00 55.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" y limit -55.00 55.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" z limit -55.00 55.00 0.20

	$jointconstrain "valvebiped.bip01_r_forearm" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" z limit -149.00 4.00 0.20

	$jointconstrain "valvebiped.bip01_r_thigh" x limit -12.00 12.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" y limit -8.00 75.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" z limit -97.00 32.00 0.20

	$jointconstrain "valvebiped.bip01_r_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" z limit -12.00 126.00 0.20

	$jointconstrain "valvebiped.bip01_r_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_foot" y limit -25.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_r_foot" z limit -15.00 35.00 0.20

	$jointconstrain "valvebiped.bip01_l_thigh" x limit -12.00 12.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" y limit -73.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" z limit -93.00 30.00 0.20

	$jointconstrain "valvebiped.bip01_l_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" z limit -8.00 126.00 0.20

	$jointconstrain "valvebiped.bip01_l_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_foot" y limit -19.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" z limit -15.00 35.00 0.20

	$jointconstrain "valvebiped.bip01_r_hand" x limit -55.00 55.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" y limit -55.00 55.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" z limit -55.00 55.00 0.20

	$jointconstrain "bonepd" x limit -90 90 0.20
	$jointconstrain "bonepd" y limit -20 20 0.20
	$jointconstrain "bonepd" z limit -20 20 0.20
}
