$cd "carebearstare\v_"
$modelname "weapons/v_carebearstare.mdl"
$body studio "reference.smd"
$cdmaterials "models\Weapons\v_hand\"
//$cdmaterials "models\Weapons\v_batte\"

//$attachment 0 "Bone032" 0.00 -0.00 -0.00 rotate 0.00 -0.00 0.00
$attachment "muzzle" "Bone031" 0.00 19.00 5.50
$attachment "fork1b" "Bone031" 0.00 19.00 5.50
$attachment "fork1m" "Bone031" 0.00 19.00 5.50
$attachment "fork1t" "Bone031" 0.00 19.00 5.50

$attachment "fork2b" "Bone031" 0.00 19.00 5.50
$attachment "fork2m" "Bone031" 0.00 19.00 5.50
$attachment "fork2t" "Bone031" 0.00 19.00 5.50

$sequence idle01 "idle" loop fps 30 activity ACT_VM_IDLE 1 rotate 0
$sequence draw "draw" snap fps 30 activity ACT_VM_DRAW 1 rotate 0