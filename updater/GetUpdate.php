<?php
function downloadFile ($url, $path) {

  $newfname = $path;
  $file = fopen ($url, "rb");
  if ($file) {
    $newf = fopen ($newfname, "wb");

    if ($newf)
    while(!feof($file)) {
      fwrite($newf, fread($file, 1024 * 8 ), 1024 * 8 );
    }
  }

  if ($file) {
    fclose($file);
  }

  if ($newf) {
    fclose($newf);
  }
 }
echo "bite";
$version = $_GET["Version"];
//$addr = $_GET["Address"];
//$key = "MULOWNZOo2";

/*if($_POST["key"] != $key)
  return;*/

phpinfo();
//downloadFile($addr . $version . "", $version . "");

//set_time_limit(0); // unlimited max execution time
/*$file = fopen( $version . '', 'w');
$options = array(
  CURLOPT_FILE => $file,
  CURLOPT_TIMEOUT => 28800, // set this to 8 hours so we dont timeout on big files
  CURLOPT_URL => $addr . $version . '',
);
var_dump($options);

$ch = curl_init();
curl_setopt_array($ch, $options);
curl_exec($ch);
echo 1;*/