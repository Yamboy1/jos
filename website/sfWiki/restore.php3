#!/usr/bin/php -q
<?php

function mostRecentSuffix() {
	global $_SITE;

	if( ! is_dir( $_SITE["backupdir"] ) ) { 
		die( $_SITE["backupdir"] . " is not a directory!\n" );
		}
	$dirhandle = opendir( $_SITE["backupdir"] );

	$bestyear = $bestday = $bestmonth = 0;
	while( $filename = readdir( $dirhandle ) ) {
		if( $filename == "." || $filename == ".." ) { continue; }
		preg_match( '/^.*\.(\d+)\.(\d+)\.(\d+)$/', $filename, $matches );

		$year = $matches[1];
		if( $year >= $bestyear ) {
			$month = $matches[2];
			if( $month >= $bestmonth ) {
				$day = $matches[3];
				if( $day > $bestday ) {
					$bestday = $day;
					$bestmonth = $month;
					$bestyear = $year;
					} /* end day */
				} /* end month */
			} /* end year */
		} /* end file loop. */

	return "$bestyear.$bestmonth.$bestday";
	} /* end mostRecentSuffix() */

function suckTable( $table, $filename ) {
	global $_SITE;

	$filename = $_SITE["backupdir"] . $filename;
	print( "Restoring from backup \"$filename\"... " );
	$mysql = "mysql -p" . $_SITE["dbpass"] . " -u " . $_SITE["dbuser"] . " " .
		$_SITE["database"] . " -h " . $_SITE["dbhost"];
	$query = "-e'LOAD DATA LOCAL INFILE \"$filename\" INTO TABLE $table IGNORE 1 LINES;'";

	$exec = $mysql . " " . $query;

	$result = system( $exec, $return );
	if( $return != 0 ) {
		die( "system call failure!\n" . $result );
		}
	print( "restoration complete.\n" );
	} /* end suckTable() */
	
/* get site-specific information */
require( "site.inc" );

/* find the most recent backup. */
$suffix = mostRecentSuffix();

/* write out topics table */
$filename = "outfile.topics." . $suffix;
suckTable( $_SITE["dbtable"], $filename );

/* write out users table */
$filename = "outfile.users." . $suffix;
suckTable( $_SITE["dbusertable"], $filename );

?>
