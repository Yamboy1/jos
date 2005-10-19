<?php
    /* could add site-specific links here. */
    $links["Wiki"] = $_SITE["docrootdir"];
    if(isset($web) && $web != "") {
    	$links[$web] = $_SITE["docrootdir"] . "view/$web/" . $web . "WebHome";
    	if(isset($topic) && $topic != "") {
		$links["[" . $topic . "]" ] = $_SITE["docrootdir"] . "edit/$web/" . urlencode($topic);
		}
	}  
?>

