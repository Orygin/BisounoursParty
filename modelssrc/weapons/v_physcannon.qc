$cd "physcannon\v_"
$modelname weapons/v_physcannon.mdl

$cdmaterials models\weapons\v_hand

$body studio "hands_reference.smd"

$origin 0 13 70

// attachment points
$attachment "muzzle" "ValveBiped.Bip01" 0.00 19.00 5.50

//Upper fork
$attachment "fork1b" "ValveBiped.Bip01_R_Finger12" 0 0 0
$attachment "fork1m" "ValveBiped.Bip01_R_Finger22" 0 0 0
$attachment "fork1t" "ValveBiped.Bip01_R_Hand" 3 -1 0

$attachment "fork2b" "ValveBiped.Bip01_L_Finger12" 0 0 0
$attachment "fork2m" "ValveBiped.Bip01_L_Finger22" 0 0 0
$attachment "fork2t" "ValveBiped.Bip01_L_Hand" 3 -1 0

$poseparameter active 0 1
$weightlist prongweights "ValveBiped.Bip01_R_Hand" 1.0 "ValveBiped.Bip01_L_Hand" 1.0

// Blend animations
$animation ProngsShut "prongs" frame 0 0 weightlist prongweights
$animation ProngsOpen "prongs" frame 1 1 weightlist prongweights

$sequence idle "idle" loop activity ACT_VM_IDLE 1
$sequence hold_idle "hold" loop activity ACT_VM_RELOAD 1
$sequence draw "Draw" snap activity ACT_VM_DRAW 1 fps 5.00
//$sequence fire "Fire" snap activity ACT_VM_PRIMARYATTACK 1
//$sequence altfire "Alt_fire" snap activity ACT_VM_SECONDARYATTACK 1

$sequence prong_blend { "ProngsShut" "ProngsOpen" blend "active" 0 1 } autoplay

//$collapsebones
