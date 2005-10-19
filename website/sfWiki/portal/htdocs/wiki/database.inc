<?php

if(! defined( "sfWikiDatabase" ) ) { 
define( "sfWikiDatabase", true );

function wikiQuery( $query ) {
	global $_SITE;

	/* Open a persistent connection to the database.
	   PHP claims it will check for extant connections
	   to this host and not reconnect automagically.
	   However, it either doesn't do this or takes forever
	   to do it.  We also know that we only ever connect
	   once, so... */
	if( $_SITE["connected"] == false || (! isset($_SITE["connected"])) ) {
		$db = mysql_pconnect( $_SITE["dbhost"], $_SITE["dbuser"], $_SITE["dbpass"] ); 
	
		if( ! $db ) {
			if( mysql_errno() == 0 ||
			    mysql_error() == "" ) {
				$error_body = "database.inc:19 -- PHP unable to report error.  ";
				} else {
				$error_body = "database.inc:19 -- " . mysql_error() . '(' . mysql_errno() . '); ';
				}
			$error_body = $error_body . "Guessing database down or site.inc misconfigured.\n";

			include( $_SITE["mirrordir"] . "query_failed.ihtml" );
			die();
			}

		if ( ! mysql_select_db( $_SITE["database"], $db ) ) {
			$error_body = "database.inc:27 -- " . mysql_error() . '(' . mysql_errno() . '); ';
			$error_body = $error_body . "Guessing site.inc misconfigured.\n";
			include( $_SITE["mirrordir"] . "query_failed.ihtml" );
			die();
			}
	
		$_SITE["connected"] = true;
		} /* end if already connected this page. */

	return mysql_query( $query );
	} /* end wikiQuery */


}
?>
