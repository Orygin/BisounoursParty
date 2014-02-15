$cd "."
$modelname "props\de_inferno\wine_barrel.mdl"
$model "Body" "wine_barrel.smd"

//KLEMS : definition des 2 skins
$texturegroup skinfamilies
{
    { "flower_barrel" }
    { "wine_barrel" }
}

$lod 30
 {
  replacemodel "wine_barrel" "lod1_wine_barrel"
}
$lod 50
 {
  replacemodel "wine_barrel" "lod2_wine_barrel"
}
$lod 70
 {
  replacemodel "wine_barrel" "lod3_wine_barrel"
}
$lod 90
 {
  replacemodel "wine_barrel" "lod4_wine_barrel"
}
$cdmaterials "models\props\de_inferno\"
$hboxset "default"
$hbox 0 "static_prop" -19.211  -19.227  -0.204  19.211  19.195  47.703
// Model uses material "flower_barrel.vmt"
$surfaceprop "Wood_Plank"
$keyvalues  { prop_data { "base" "Wooden.Barrel"  } } 
$illumposition 0.000 -0.016 23.749
$sequence idle "idle" fps 30.00
$collisionmodel "phymodel.smd" {

	$concave
	$mass 75.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
$collisiontext {
	break { "model" "props\de_inferno\wine_barrel_p1" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p2" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p3" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p4" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p5" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p6" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p7" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p8" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p9" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p10" "fadetime" "10"}
	break { "model" "props\de_inferno\wine_barrel_p11" "fadetime" "10"}
}
