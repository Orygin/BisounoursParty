$cd "."
$modelname "props_bisou/canister.mdl"
$model "Body" "canister01a_reference.smd"
$lod 30
{
  replacemodel "canister01a_reference" "lod1_canister01a_reference"
}
$cdmaterials "models\props_bisou\canister"

$texturegroup skinfamilies
{
    { "canister_pink" }
    { "canister_green" }
}

$hboxset "default"
$hbox 1 "static_prop" -5.280  -5.410  -29.500  5.280  5.410  21.500
$hbox 1 "static_prop" -2.500  -2.500  21.500  2.500  2.500  29.500

$attachment "nozzle" "static_prop" 0.00 0.00 20.00 rotate 0.00 90.00 0.00

$surfaceprop "canister"
$keyvalues  
{
	prop_data
	{
		"base" "Metal.Medium"
		"dmg.bullets" "2.0"
		"dmg.club" "100.0"
		"dmg.explosive" "1.0"
		"health" "80"
		"explosive_damage" "200"
		"explosive_radius" "300"
	}
	fire_interactions
	{
		"explosive_resist" "yes"
		"flammable" "yes"
		"ignite" "halfhealth"
	}
}

$illumposition -0.201 0.000 0.000
$sequence idle "idle" fps 30.00
$collisionmodel "phymodel.smd" {

	$mass 60.0
	$inertia 1.00
	$damping 0.00
	$rotdamping 0.00
}
$collisiontext {
	break { "model" "props_c17/canisterchunk01a" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01b" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01c" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01d" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01f" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01g" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01h" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01i" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01k" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01l" "health" "10" "fadetime" "5"}
	break { "model" "props_c17/canisterchunk01m" "health" "10" "fadetime" "5"}
}
