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

/* generate the topic table */
$query ="CREATE TABLE " . $_SITE["dbtable"] . " (" .
              "topic VARCHAR(255) PRIMARY KEY, " .
              "web VARCHAR(255), summary VARCHAR(64), " .
              "lastedit DATETIME, " .
	      "lasteditor VARCHAR(64), " .
	      "tstamp TIMESTAMP(14), " .
              "linksfrom MEDIUMBLOB, " .
	      "lfcount MEDIUMINT, " . 
              "rendertext LONGBLOB, " .
	      "full_text LONGBLOB, " .
	      "old_full_text LONGBLOB, " . 
	      "ltcount MEDIUMINT " . ");";
reportErrors( $query, $db, "Topic table created successfully.\n" );

/* generate the session data storage table */
$query = "CREATE TABLE " . $_SITE["dbsessiontable"] . " ( " .
		"session_id VARCHAR(64) NOT NULL PRIMARY KEY, " .
		"session_date INTEGER NOT NULL, " .
		"session_data TEXT " . ");";
reportErrors( $query, $db, "Session table created successfully.\n" );

$query = "CREATE TABLE " . $_SITE["dbusertable"] . " ( " .
		"username VARCHAR(64) NOT NULL PRIMARY KEY, " .
		"password VARCHAR(64) NOT NULL, " .
		"email VARCHAR(64) NOT NULL " . ");";
reportErrors( $query, $db, "User table created successfully.\n" );

?>
