#!/usr/bin/php -q
<?php
/* get site-specific information */ 
require( "../site.inc" );

include( $_SITE["mirrordir"] . "web_util.inc" );
include( $_SITE["mirrordir"] . "view.inc" );

$result = wikiQuery( "SELECT topic FROM " . $_SITE["dbtable"] . ";" );

while ( $row = mysql_fetch_array( $result ) ) {
	$topic = $row["topic"];
	print( "Touching \"$topic\"...\n" );
	$web = verifyTopicWeb( $topic );
	checkRendering( $topic, $web, false );
	}
?>
