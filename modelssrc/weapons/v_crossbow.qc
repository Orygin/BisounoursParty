$cd "crossbow\v_"
$modelname "Weapons/v_crossbow.mdl"
$body studio "Crossbow_reference.smd"
$cdmaterials "models\weapons\v_crossbow\"
$cdmaterials "models\Weapons\v_hand\"

$texturegroup skinfamilies
{
	{ "engrenage_marche.vmt" }
 	{ "engrenage_arret.vmt" }
}

$attachment "spark" "Crossbow_model.Spark2" 0.50 -0.00 0.00 rotate -90.00 0.00 0.00
$attachment "bolt_start" "Crossbow_model.bolt" 0.00 -0.00 16.00 rotate -90.00 0.00 0.00
$attachment "bolt_end" "Crossbow_model.bolt" 0.00 0.00 -4.00 rotate -90.00 0.00 0.00

$sequence idle "idle" loop fps 30 activity ACT_VM_IDLE 1 node Ready
$sequence idle_empty "idle_empty" loop fps 30 activity ACT_VM_FIDGET 1 node Ready
$sequence draw "draw" fps 90.00 snap activity ACT_VM_DRAW 1 node Ready
$sequence fire "fire" snap activity ACT_VM_PRIMARYATTACK 1 fps 30.00 node Ready
$sequence reload "reload" ACT_VM_RELOAD 1 fps 30.00 node Ready
