#!/usr/bin/php -q
<?php
/* get site-specific information */ 
require( "../site.inc" );

/* Required for saveWikiTopic() */
include( $_SITE["mirrordir"] . "edit.inc" );
include( $_SITE["mirrordir"] . "view.inc" );
include( $_SITE["mirrordir"] . "web_util.inc" );
include( $_SITE["mirrordir"] . "web_format.inc" );

$query = "SELECT topic,full_text,summary FROM " . $_SITE["dbtable"] . " WHERE web != \"Imported\" ;";
print( $query . "\n" );
$result = wikiQuery( $query );

while( $row = mysql_fetch_array( $result ) ) {
	if( file_exists( "../import/Main/" . $row["topic"] . ".txt" ) ) {
		$topic = $row["topic"];
		$summary = $row["summary"];

		print( "Moving $topic to Imported... " );
		if ( ! saveWikiTopic( $topic, "Imported", $row["full_text"], $summary, false ) ) {
			print( "failed.\n" ); $failed = $failed . " " . $topic; } else { print( "done.\n" ); }
		} /* end moving. */
	} /* end loop */

print( "Failed:" . $failed . "\n" );

?>
