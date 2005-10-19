#!/usr/bin/php -q
<?php
/* get site-specific information */ 
require( "../site.inc" );

/* Required for saveWikiTopic() */
include( $_SITE["mirrordir"] . "edit.inc" );
include( $_SITE["mirrordir"] . "view.inc" );
include( $_SITE["mirrordir"] . "web_util.inc" );
include( $_SITE["mirrordir"] . "web_format.inc" );

function insertFileIntoDatabase( $topicName, $filename ) {
	global $_SITE;

	/* open file and read it and close it */
	$file = fopen( $filename, "r" );
	if( ! $file ) { die( "Unable to open $filename." ); }
	$text = fread( $file, filesize( $filename ) );
	fclose( $file );

	$mtime = filemtime( $filename );
	$sqltime = date( "Y-m-d H:i:s", $mtime );
	
	/* Do the import; could change "Main" to "Imported"... */
	if( ! saveWikiTopic( $topicName, "Imported", $text, "Imported from another Wiki.", true ) ) {
		die( "Unable to save WikiTopic \"$topicName\"\n" );
		}

	/* Change the last-edited time to when the file was last changed. */
	wikiQuery( "UPDATE " . $_SITE["dbtable"] . " SET lastedit = \"" . $sqltime . "\" WHERE topic=\"" . $topicName . "\";" );  
	/* Make the 'revert' copy. */
	wikiQuery( "UPDATE " . $_SITE["dbtable"] . " SET old_full_text = \"" . $text . "\" WHERE topic=\"" . $topicName . "\";" );

	/* Rename the file so that we don't suck it again if we have
	   to start over from a time-out. */
	if( ! rename( $filename, $filename . ".imp" ) ) {
		die( "Failed to rename \"$filename\"\n" );
		}
	} /* end insertFileIntoDatabase() */

function recursiveSuck( $dirname ) {
	if( ! is_dir( $dirname ) ) { die( "$dirname not a directory." ); }

	$dirhandle = opendir( $dirname );
	while( $filename = readdir( $dirhandle ) ) {
	        if( $filename == "." || $filename == ".." ) { continue; }
		if( preg_match( "/.*\.txt$/", $filename ) ) {
			$topicName = preg_replace( "/(.*)\.txt/", "\\1", $filename );
	        	print( "importing $dirname$filename as $topicName...\n" );
			insertFileIntoDatabase( $topicName, $dirname . $filename );
			}
		} /* end directory read */
	closedir( $dirhandle );
	} /* end recursiveSuck */

$dir = "../../sfWiki/import/Main/";
insertFileIntoDatabase( "JOS", $dir . "JOS.txt" );
insertFileIntoDatabase( "jos", $dir . "jos.txt" );
insertFileIntoDatabase( "KAFFE", $dir . "KAFFE.txt" );
insertFileIntoDatabase( "Kaffe", $dir . "Kaffe.txt" );
insertFileIntoDatabase( "classpath", $dir . "classpath.txt" );
insertFileIntoDatabase( "Classpath", $dir . "Classpath.txt" );
insertFileIntoDatabase( "KORE", $dir . "KORE.txt" );
insertFileIntoDatabase( "Kore", $dir . "Kore.txt" );

?>
