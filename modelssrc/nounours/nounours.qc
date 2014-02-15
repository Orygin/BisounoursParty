$cd "."
$modelname "items/nounours.mdl"

$cdmaterials "models/items"
$surfaceprop "carpet"
$keyvalues { "prop_data" { "base" "Cloth.Large" } }
$scale 1.0

$body studio "nounours.smd"
$texturegroup skinfamilies
{
    { "nounours_rose" }
    { "nounours_vert" }
}
$sequence idle "idle" fps 1
$sequence ragdoll "ragdoll" fps 1 ACT_IDLE 1

$collisionjoints "nounours_physic.smd" { 

   $rootbone "root" 
   $concave
 
   $mass 0.1
   $inertia 5.00
   $damping 0.01
   $rotdamping 1.50
 
   $jointconstrain body x limit -45.000000 45.000000 1.000000
   $jointconstrain body y limit -20.000000 20.000000 1.000000
   $jointconstrain body z limit -8.000000 8.000000 1.000000
   $jointconstrain head x limit -30.000000 30.000000 1.000000
   $jointconstrain head y limit -25.000000 25.000000 1.000000
   $jointconstrain head z limit 0.000000 0.000000 0.000000
   $jointconstrain leftarm x limit -30.000000 30.000000 1.000000
   $jointconstrain leftarm y limit -25.000000 25.000000 1.000000
   $jointconstrain leftarm z limit 0.000000 0.000000 0.000000
   $jointconstrain rightarm x limit -30.000000 30.000000 1.000000
   $jointconstrain rightarm y limit -25.000000 25.000000 1.000000
   $jointconstrain rightarm z limit 0.000000 0.000000 0.000000
   $jointconstrain leftleg x limit -30.000000 30.000000 1.000000
   $jointconstrain leftleg y limit -25.000000 25.000000 1.000000
   $jointconstrain leftleg z limit 0.000000 0.000000 0.000000
   $jointconstrain rightleg x limit -30.000000 30.000000 1.000000
   $jointconstrain rightleg y limit -25.000000 25.000000 1.000000
   $jointconstrain rightleg z limit 0.000000 0.000000 0.000000 
}