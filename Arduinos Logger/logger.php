<?php
	$file = fopen("logs.txt", "a") or die("Unable to read!");
	$txt = $_GET['first'] . $_GET['second']."\n";
	fwrite($file, $txt); fclose($file);
?>