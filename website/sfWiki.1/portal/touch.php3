#!/usr/bin/php -q
<?php
/* get site-specific information */ 
require( "../site.inc" );
require( $_SITE["mirrordir"] . "database.inc" );
require( $_SITE["mirrordir"] . "edit.inc" );

$result = wikiQuery( "SELECT topic,web,full_text,summary FROM " . $_SITE["dbtable"] . ";" );
while ( $tarray = mysql_fetch_array( $result ) ) {
	saveWikiTopic( $tarray["topic"], $tarray["web"], $tarray["full_text"], $tarray["summary"], false );
	}

?>
