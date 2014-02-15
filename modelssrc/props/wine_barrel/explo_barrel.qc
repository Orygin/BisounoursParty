$scale "0.80"
$cd "."
$modelname "props\de_inferno\explo_barrel.mdl"
$model "Body" "wine_barrel.smd"

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
$cdmaterials "models\props\de_inferno\explo\"
$hboxset "default"
$hbox 0 "static_prop" -19.211  -19.227  -0.204  19.211  19.195  47.703
// Model uses material "flower_barrel.vmt"
$surfaceprop "Wood_Plank"
$keyvalues  { prop_data { "base" "Wooden.Barrel" "dmg.bullets" "1.0" "dmg.club" "1.0" "dmg.explosive" "1.0" "health" "20" "explosive_damage" "170" "explosive_radius" "300"  } physgun_interactions { "onfirstimpact" "break"  } fire_interactions { "flammable" "yes" "explosive_resist" "yes" "ignite" "halfhealth"  } } 
$illumposition 0.000 -0.016 23.749
$sequence idle "idle" fps 30.00
$collisionmodel "phymodel.smd" {

	$concave
	$mass 75.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
