<?php
set_time_limit(0);

$version = $_GET["Version"];
$file = $version . ".update";
$remote_file = 'public_html/'. $version .'.update';

// set up basic connection
$conn_id = ftp_connect("bpmirror.netai.net");

// login with username and password
$login_result = ftp_login($conn_id, "a9379579", "*");

// upload a file
if (ftp_put($conn_id, $remote_file, $file, FTP_BINARY)) {
 echo "successfully uploaded $file\n";
} else {
 echo "There was a problem while uploading $file\n";
}

ftp_close($conn_id);