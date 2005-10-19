#!/usr/bin/php -q
<?php

function dumpTable( $table, $filename ) {
	global $_SITE;	

	$filename = $_SITE["backupdir"] . $filename;
	print( "Backing up to \"$filename\"... " );
	$prefix = "mysql -p" . $_SITE["dbpass"] . " -u " . $_SITE["dbuser"] . " " .
		$_SITE["database"] . " -h " . $_SITE["dbhost"] . " -e'";
	$suffix = "'>";
	
	$query = "SELECT * FROM $table;";
	$exec = $prefix . $query . $suffix . $filename;

	$result = system( $exec, $return );
	if( $return != 0 ) {
		die( "system call failure.\n" . $result );
		}
	print( "backup complete.\n" );
	} /* end dumpTable() */
	
/* get site-specific information */
require( "site.inc" );

/* the MySQL 'CGI' command */

$datestamp = strftime( "%Y.%m.%d" );

/* write out topics table */
$filename = "outfile.topics." . $datestamp;
dumpTable( $_SITE["dbtable"], $filename );

/* write out users table */
$filename = "outfile.users." . $datestamp;
dumpTable( $_SITE["dbusertable"], $filename );

?>
