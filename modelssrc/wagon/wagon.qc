$cd "."
$origin 0 0 -10.4
$modelname "props_bisou/wagon.mdl"
$body studio "wagon_ref.smd"
$cdmaterials "models/props_bisou"

$texturegroup skinfamilies
{
    { "wagon" }
    { "wagon_off" }
}

$surfaceprop "metal"

$sequence "Idle" "wagon_idle" loop fps 30
$sequence "Marche05" "wagon_idle2" loop fps 5
$sequence "Marche10" "wagon_idle2" loop fps 10
$sequence "Marche20" "wagon_idle2" loop fps 20
$sequence "Marche30" "wagon_idle2" loop fps 30
$sequence "Marche40" "wagon_idle2" loop fps 40
$sequence "Marche50" "wagon_idle2" loop fps 50

$collisionmodel "wagon_phys.smd" {
$concave
}