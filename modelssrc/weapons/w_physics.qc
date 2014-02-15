$cd "physcannon\w_"

$modelname "weapons/w_physics.mdl"
$body studio "w_physics.smd"
$cdmaterials "models\weapons\w_physics\"

$attachment "core" "ValveBiped.Bip01_Spine" 0 15.5 0
$attachment "fork1m" "ValveBiped.Bip01_Spine" 0 15.5 0
$attachment "fork1t" "ValveBiped.Bip01_Spine" 0 15.5 0
$attachment "fork2m" "ValveBiped.Bip01_Head1" 9 -3.5 2.30
$attachment "fork2t" "ValveBiped.Bip01_R_Hand" 6 -2 2
$attachment "fork3m" "ValveBiped.Bip01_Head1" 9 -3.5 -2
$attachment "fork3t" "ValveBiped.Bip01_L_Hand" 6 -2 2

$sequence idle "idle" loop ACT_VM_IDLE 1 fps 30.00
