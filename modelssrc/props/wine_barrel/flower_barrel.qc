$cd "."
$modelname "props\de_inferno\flower_barrel.mdl"
$model "Body" "flower_barrel.smd"
$lod 30
 {
  replacemodel "flower_barrel" "lod1_flower_barrel"
}
$lod 50
 {
  replacemodel "flower_barrel" "lod2_flower_barrel"
}
$lod 70
 {
  replacemodel "flower_barrel" "lod3_flower_barrel"
}
$lod 90
 {
  replacemodel "flower_barrel" "lod4_flower_barrel"
}
$cdmaterials "models\props\de_inferno\"
$hboxset "default"
$hbox 0 "static_prop" -31.978  -32.088  -0.198  31.355  33.152  55.966
// Model uses material "flower_barrel.vmt"
$surfaceprop "wood_plank"
$keyvalues  { prop_data { "base" "Wooden.Barrel"  } } 
$illumposition -0.312 0.532 27.884
$sequence idle "idle" act_idle 1 fps 30.00
$collisionmodel "phymodel.smd" {

	$mass 200.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
$collisiontext {
	break { "model" "props\de_inferno\flower_barrel_p1" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p2" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p3" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p4" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p5" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p6" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p7" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p8" "health" "1" "fadetime" "5"}
	break { "model" "props\de_inferno\flower_barrel_p9" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p10" "health" "1" "fadetime" "10"}
	break { "model" "props\de_inferno\flower_barrel_p11" "health" "1" "fadetime" "10"}
}
