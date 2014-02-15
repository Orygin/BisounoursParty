$cd "."
$modelname "props\de_inferno\wine_barrel_static.mdl"
$model "Body" "wine_barrel.smd"
$cdmaterials "models\props\de_inferno\"

$texturegroup skinfamilies
{
    { "flower_barrel" }
    { "wine_barrel" }
    { "explo\flower_barrel" }
    { "paint_pink_barrel" }
    { "paint_green_barrel" }
}

$lod 20
 {
  replacemodel "wine_barrel" "lod1_wine_barrel"
}
$lod 30
 {
  replacemodel "wine_barrel" "lod2_wine_barrel"
}
$lod 50
 {
  replacemodel "wine_barrel" "lod3_wine_barrel"
}
$lod 70
 {
  replacemodel "wine_barrel" "lod4_wine_barrel"
}

$staticprop
$surfaceprop "Wood_Plank"

$sequence "idle" "wine_barrel" fps 30.00

$collisionmodel "phymodel.smd"
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