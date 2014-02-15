$cd "."

$modelname "props\cs_office\radio.mdl"
$model "Body" "radio.smd"
$cdmaterials "models\props\cs_office\"

$staticprop
$surfaceprop "computer"

$sequence idle "radio" loop fps 30.00
$collisionmodel "radio_phy.smd"