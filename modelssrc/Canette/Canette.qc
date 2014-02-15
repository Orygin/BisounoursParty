$scale 1.844
$modelname	"props\canette.mdl"
$body canette	"Canette.smd"
$staticprop
$surfaceprop	Metal_Box

$collapsebones
$sequence idle	"Canette.smd"
$hboxset "default"
$collisionmodel	"Canette.smd" { $concave }

$texturegroup skinfamilies
{
    { "canette_pink" }
    { "canette_green" }
}
$cdmaterials	"models\props"