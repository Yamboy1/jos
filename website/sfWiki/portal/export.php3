#!/usr/bin/php -q
<?php
/* get site-specific information */ 
require( "../site.inc" );

include( $_SITE["mirrordir"] . "web_util.inc" );
include( $_SITE["mirrordir"] . "view.inc" );

function writeTopicFromDatabase( $topic, $web, $directory ) {
	global $_SITE;

	/* suck topic from database. */
	if( ! $result = FetchWikiEntry( $topic, $web, false ) ) {
		die( "Unable to locate \"$topic\".\n" );
		}

	$row = mysql_fetch_array( $result );
	$fulltext = stripslashes($row["full_text"]);

	/* open and verify file */
	$file = fopen( $directory . $topic . ".txt", "w" );
	if( ! $file ) { die( "Unable to open $filename." ); }

	/* should adjust mtime so that it works right for import.php3 */
	$text = fwrite( $file, $fulltext );
	fclose( $file );
	} /* end writeTopicFromDatabase() */

$stdin = fopen( "php://stdin", "r" );
if( ! $stdin ) { die( "Unable to open standard in.\n" ); }

print( "Which WikiTopic do wish to export?  Type [all] to export every WikiTopic.\n" );
$response = chop( fgets( $stdin, 1024 ) );
if( $response == "[all]" ) {
	$result = wikiQuery( "SELECT topic FROM " . $_SITE["dbtable"] . ";" );

	while ( $row = mysql_fetch_array( $result ) ) {
		$topic = $row["topic"];
		print( "Exporting $topic...\n" );
		writeTopicFromDatabase( $topic, "Main", "../export/" );
		}
	} else {
	writeTopicFromDatabase( $response, "Main", "../export/" );
	}
?>
