$cd "."

$modelname "player\carebear_cinematic.mdl"
$root "pelvis"

$eyeposition 0.000 0.000 70.000
$attachment "eyes" "ValveBiped.Bip01_Head1" 0.000 -2.111 73.068 absolute


//Taunts ?
$sequence sleep "sleep" fps 30


$model "Carebear" "carebear_reference.smd" {
	eyeball righteye ValveBiped.Bip01_Head1 -2.006 -2.111 73.068 eye/eyeball_r 1.000  2.000 eye/pupil_r 1.900
	eyeball lefteye ValveBiped.Bip01_Head1 2.179 -2.111 73.068 eye/eyeball_l 1.000  -2.000 eye/pupil_l 1.900
	$include "bodyrules_xsi.qci"
}

$texturegroup skinfamilies
{
    { "rose/vie"
	 "eye/eyeball_l" 
	 "eye/eyeball_r" }
    { "rose/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "vert/vie"
	 "eye/eyeball_l" 
	 "eye/eyeball_r" }
    { "vert/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "rouge/vie"
	 "eye/eyeball_l2" 
	 "eye/eyeball_r2" }
    { "rouge/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "orange/vie"
	 "eye/eyeball_l2" 
	 "eye/eyeball_r2" }
    { "orange/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "bleu/vie"
	 "eye/eyeball_l2" 
	 "eye/eyeball_r2" }
    { "bleu/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "violet/vie"
	 "eye/eyeball_l2" 
	 "eye/eyeball_r2" }
    { "violet/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "jaune/vie"
	 "eye/eyeball_l2" 
	 "eye/eyeball_r2" }
    { "jaune/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "dev/vie"
	 "eye/bonus" 
	 "eye/bonus" }
    { "dev/mort"
	 "eye/noeye" 
	 "eye/noeye" }

    { "bonus/rose"
	 "eye/bonus" 
	 "eye/bonus" }
    { "bonus/vert"
	 "eye/bonus" 
	 "eye/bonus" }
}

$surfaceprop "carebear"
$jointsurfaceprop "valvebiped.bip01_head1" "carebear_head"

$include "standardhierarchy.qci"
//KLEMS : parait que les IK chain ne marchent pas en multiplayer, inutile de les ajouter alors (cpt obvious)
$include "standardikchains.qci"
$include "commonbones.qci"
$proceduralbones "male.vrd"
$bbox -13 -13 0  13 13 72

$realignbones
$collapsebones

$lod 25
{
	replacemodel "carebear_reference" "lod1_carebear_reference"
}

$lod 70
{
	replacemodel "carebear_reference" "lod1_carebear_reference"
	removemesh "models/player/eye/eyeball_r"
	removemesh "models/player/eye/eyeball_l"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$shadowlod
{
	replacemodel "carebear_reference" "lod1_carebear_reference"
	removemesh "models/player/eye/eyeball_r"
	removemesh "models/player/eye/eyeball_l"
	bonetreecollapse "ValveBiped.Bip01_L_Forearm"
	bonetreecollapse "ValveBiped.Bip01_R_Forearm"
	bonetreecollapse "ValveBiped.Bip01_L_Calf"
	bonetreecollapse "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2"
	replacebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine"
	replacebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Spine2"
}

$cdmaterials "models\player\"

$hboxset "default"
$hbox 0 "ValveBiped.Bip01_Spine"	    -1.00   -2.00   -9.00     7.00   12.00    9.00
$hbox 0 "ValveBiped.Bip01_Head1"	    -2.00   -4.00   -8.50    14.00   10.00    8.50
$hbox 0 "ValveBiped.Bip01_L_UpperArm"	-2.00   -2.75   -2.75    12.00    2.75    2.75
$hbox 0 "ValveBiped.Bip01_L_Forearm"	 0.00   -2.25   -2.25    10.00    2.25    2.25
$hbox 0 "ValveBiped.Bip01_L_Hand"	     0.50   -1.00   -2.00     5.50    1.00    2.00
$hbox 0 "ValveBiped.Bip01_R_UpperArm"	-2.00   -2.75   -2.75    12.00    2.75    2.75
$hbox 0 "ValveBiped.Bip01_R_Forearm"	 0.00   -2.25   -2.25    10.00    2.25    2.25
$hbox 0 "ValveBiped.Bip01_R_Hand"	     0.50   -1.00   -2.00     5.50    1.00    2.00
$hbox 0 "ValveBiped.Bip01_L_Thigh"	     1.00   -2.00   -3.50    19.00    8.00    6.50
$hbox 0 "ValveBiped.Bip01_L_Calf"	     0.50   -2.00   -2.60    15.50    6.00    6.40
$hbox 0 "ValveBiped.Bip01_L_Foot"	    -2.00   -1.50   -2.00     6.00    4.50    6.00
$hbox 0 "ValveBiped.Bip01_R_Thigh"	     1.00   -2.00   -6.50    19.00    8.00    3.50
$hbox 0 "ValveBiped.Bip01_R_Calf"	     0.50   -2.00   -6.40    15.50    6.00    2.60
$hbox 0 "ValveBiped.Bip01_R_Foot"	    -2.00   -1.50   -6.00     6.00    4.50    2.00
$hbox 0 "ValveBiped.Bip01_Pelvis"	   -11.50   -4.00   -4.00    11.50    4.00    8.00
$hbox 0 "ValveBiped.Bip01_Spine2"	    -1.00   -1.00   -7.50    13.00    7.00    7.50
// Hitboxes du dos, definies en "head" pour les dommages
//$hbox 1 "ValveBiped.Bip01_Head1"	    -3.00   10.00   -7.50    13.00   12.00    7.50
//$hbox 1 "ValveBiped.Bip01_Spine2"	    -4.50   -8.00   -8.00    12.50   -2.00    8.00
//$hbox 1 "ValveBiped.Bip01_Spine"	    -2.00   -8.50   -9.50     8.00    0.50    9.50
//$hbox 1 "ValveBiped.Bip01_Pelvis"	   -12.00   -4.50  -12.00    12.00    4.50   -4.00
//$hbox 1 "ValveBiped.Bip01_L_Thigh"	     1.00    7.00   -4.00    15.00    9.00    6.00
//$hbox 1 "ValveBiped.Bip01_R_Thigh"	     1.00    7.00   -6.00    15.00    9.00    4.00
//$hbox 1 "ValveBiped.Bip01_R_Calf"	     6.50    6.00   -6.50    15.50    8.00    2.50
//$hbox 1 "ValveBiped.Bip01_L_Calf"	     6.50    6.00   -2.50    15.50    8.00    6.50
//$hbox 1 "ValveBiped.Bip01_Spine1"	   -12.50  -11.00  -11.00    12.50   -1.00    7.00

$attachment "lefthand" "ValveBiped.Bip01_L_Hand" 0.00 -0.00 -0.00 rotate -0.00 0.00 -0.00
$attachment "zipline" "ValveBiped.Bip01_Spine4" 3.00 -4.00 -0.00 rotate 0.00 -0.00 -0.00
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" -0.00 -0.00 -0.00 rotate -90.00 -90.00 0.00
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" -0.00 -0.00 0.00 rotate -90.00 -90.00 0.00

//KLEMS : attachment pour le ragdoll firstperson
$attachment "first" "ValveBiped.Bip01_Head1" 4.50 8.00 0.00 rotate 0.00 -80.00 -90.00

//KLEMS : animations custom pour avoir des vraie animations sur les npc humanoides
//$includemodel "player/m_anm.mdl"
//$includemodel "player/m_gst.mdl"
//$includemodel "player/m_pst.mdl"
//$includemodel "player/m_shd.mdl"
//$includemodel "player/m_ss.mdl"

//$includemodel "combine_soldier_anims.mdl"
//$includemodel "male_anims.mdl"
//$includemodel "Police_animations.mdl"
//$includemodel "Police_ss.mdl"
$includemodel "humans/male_shared.mdl"
$includemodel "humans/male_ss.mdl"
$includemodel "humans/male_gestures.mdl"
$includemodel "humans/male_postures.mdl"
//$includemodel "humans/female_shared.mdl"
//$includemodel "humans/female_ss.mdl"
//$includemodel "humans/female_gestures.mdl"
//$includemodel "humans/female_postures.mdl"

$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00

//$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.707 0.707 0.000
//$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.707 0.707 0.000
//$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.707 -0.707 0.000
//$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.707 -0.707 0.000

$collisionjoints "phymodel.smd" {

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
