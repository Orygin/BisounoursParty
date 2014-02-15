$scale 12.908
$modelname	"props\flag_small.mdl"
$body flag	"reference.smd"

$collapsebones

$sequence idle "reference.smd"
$sequence fly "flag1.smd" loop
$sequence flyslow "flag1.smd" loop fps 20

$hboxset "default"
$collisionmodel	"reference.smd" { $concave }

$texturegroup skinfamilies
{
    { "flag_pink" }
    { "flag_green" }
}

$cdmaterials	"models\props"