/*$cd "rpg\v_"
$modelname "Weapons/v_rpg.mdl"
$body studio "RPG_reference.smd"
$cdmaterials "Models\Weapons\v_rpg\"
$cdmaterials "models\Weapons\v_hand\"
$origin -3 0 0

$attachment "0" "muzzle" -0.00 -0.00 -0.00 rotate -0.00 0.00 -0.00
$attachment "1" "muzzle" -55.00 0.00 0.00 rotate -0 -180 -180

$sequence idle1 "Idle1" loop fps 30 activity ACT_VM_IDLE 1 node Idle
$sequence draw "Draw" fps 70 snap activity ACT_VM_DRAW 1 node Idle
$sequence fire "fire" snap ACT_VM_PRIMARYATTACK 1 fps 30.00 node Fire {
  { event 5001 0 }
  { event 6001 0 "3" }
}

==============================================================================

Rocket launcher

==============================================================================
*/
$cd "rpg\v_"
$modelname "Weapons/v_rpg.mdl"
$body studio "RPG_reference.smd"
$cdmaterials "Models\Weapons\v_rpg\"
$cdmaterials "models\Weapons\v_hand\"

$sequence idle1 "Idle1" loop fps 30 activity ACT_VM_IDLE 1 node Idle
$sequence draw "Draw" fps 90 snap activity ACT_VM_DRAW 1 node Idle
$sequence fire "Fire" fps 30 snap activity ACT_VM_PRIMARYATTACK 1 { { event 5001 0 } { event 6001 0 "3" } } Node Fire