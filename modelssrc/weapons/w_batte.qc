$cd "batte\w_"
// $cd "C:\Documents and Settings\Thomas\Bureau\bisoumod\crowbar\w_"
$modelname "weapons/w_batte.mdl"
$model "studio" "w_Crowbar_reference.smd"
// $lod 15
//  {
//   replacemodel "w_Crowbar_reference" "lod1_w_Crowbar_reference"
// }
$cdmaterials "models\weapons\w_batte\"
$hboxset "default"
$hbox 0 "ValveBiped.Bip01_R_Hand" 0.000  -3.636  -25.478  7.125  0.000  8.861
// Model uses material "head_uvw.vmt"
// Model uses material "crowbar_cyl.vmt"
$surfaceprop "Wood_Solid"
$illumposition -1.456 -0.510 0.723
$sequence idle "idle" loop ACT_VM_IDLE 1 fps 30.00
$sequence attack1 "attack1" fps 30.00
$collisionmodel "phymodel.smd" {

	$mass 8.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
