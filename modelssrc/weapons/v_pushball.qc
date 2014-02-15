$cd "pushball"
$modelname "Weapons/v_pushball.mdl"
$body studio "pushball_reference.smd"
$cdmaterials "models\Weapons\v_hand\"
$cdmaterials "models\items\push\"
$zbrush

$attachment "0" "Bone031" 0.00 -0.00 -0.00 rotate -0.00 -0.00 90.00

$sequence idle01 "Idle01" loop fps 30 activity ACT_VM_IDLE 1 rotate 0
$sequence draw "draw" fps 90 snap activity ACT_VM_DRAW 1 rotate 90
$sequence misscenter1 "misscenter1" fps 30 snap activity ACT_VM_MISSCENTER 1 rotate 90
$sequence misscenter2 "misscenter2" fps 30 snap activity ACT_VM_MISSCENTER 1 rotate 90
$sequence hitcenter1 "hitcenter1" fps 30 snap activity ACT_VM_HITCENTER 1 rotate 90
$sequence hitcenter2 "hitcenter2" fps 30 snap activity ACT_VM_HITCENTER 1 rotate 90
$sequence hitcenter3 "hitcenter3" fps 30 snap activity ACT_VM_HITCENTER 1 rotate 90
$sequence hitkill1 "Hitkill1" fps 30 snap activity ACT_VM_HITKILL 1 rotate 90
