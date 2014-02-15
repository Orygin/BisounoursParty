$cd "rpg\w_"
$modelname "weapons/w_rpg.mdl"
$model "studio" "w_rpg_reference.smd"
//$lod 35
// {
//  replacemodel "w_rpg_reference" "lod1_w_rpg_reference"
//}
$cdmaterials "models\Weapons\w_rpg\"
$hboxset "default"
$hbox 0 "ValveBiped.Anim_Attachment_RH" -3.598  -3.527  -27.897  3.335  12.731  16.269
// Model uses material "w_rpg_sheet.vmt"
$attachment "muzzle" "ValveBiped.Anim_Attachment_RH" -1.00 11.50 15.00 rotate -77 90 180
$attachment "1" "ValveBiped.Anim_Attachment_RH" -1.00 3.00 -23.00 rotate 77 -90 -90
$attachment "missile" "ValveBiped.Anim_Attachment_RH" 0.00 13.00 23.00 rotate -77 90 -180
$attachment "exhaust" "ValveBiped.Anim_Attachment_RH" -1.00 3.00 -23.00 rotate 77 -90 -180
$surfaceprop "weapon"
$illumposition 18.753 -1.525 5.383
$sequence idle "idle" ACT_VM_IDLE 1 fps 30.00
$sequence attack1 "attack1" ACT_RANGE_ATTACK_RPG 1 fps 30.00 {
  { event AE_NPC_MUZZLEFLASH 0 "RPG MUZZLE" }
  { event 3002 0 }
  { event AE_NPC_MUZZLEFLASH 0 "RPG EXHAUST" }
  { event 6001 0 "3" }
}
$sequence fire "attack1" ACT_RANGE_ATTACK_RPG 1 fps 30.00 {
  { event 3002 0 }
  { event 6001 0 "3" }
}

$collisionmodel "phymodel.smd" {
	$mass 20.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
