<?php
    /* could add site-specific links here. */
    $links["Wiki"] = $_SITE["docrootdir"];
    if(isset($web) && $web != "") {
    	$links[$web] = $_SITE["docrootdir"] . "view.php?web=$web&topic=" . $web . "WebHome";
    	if(isset($topic) && $topic != "") {
		$links["[" . $topic . "]" ] = $_SITE["docrootdir"] . "edit.php?web=$web&topic=" . urlencode($topic);
		}
	}  
?>

