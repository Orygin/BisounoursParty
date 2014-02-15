$scale 1.0
$cd "."
$modelname props_bisou/reveil.mdl
$model "Body" "reveil.smd"
$cdmaterials "models/props_bisou/reveil"

$texturegroup skinfamilies
{
    { "reveil"
	 "chronometre" }

    { "reveil"
	 "chronometre3" }
}

//$staticprop
$surfaceprop "metal"

$sequence "idle" "reveil" fps 30.00

$collisionmodel "reveil"