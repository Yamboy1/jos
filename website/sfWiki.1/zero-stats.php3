#!/usr/bin/php -q
<?php

function reportErrors( $query, $database, $success ) {
	/* execute the query */
	$result = mysql_query( $query, $database );

	/* report what happened */
	$error = mysql_error();
	if( $error ) {
		print( mysql_error() . "\n" );
		} else {
		print( $success );
		}
	}

/* get site-specific information */
require( "site.inc" );

/* connect to the database */
$db = mysql_connect( $_SITE["dbhost"], $_SITE["dbuser"], $_SITE["dbpass"] );
mysql_select_db( $_SITE["database"], $db );

$query = "REPLACE INTO " . $_SITE["dbstattable"] . " (topic) SELECT topic FROM " . $_SITE["dbtable"] . ";";
reportErrors( $query, $db, "Statistics table initialized successfully.\n" );

?>
