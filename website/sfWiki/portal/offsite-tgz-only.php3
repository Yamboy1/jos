#!/usr/bin/php -q
<?php
include( "../site.inc" );

/* configuration */
$onsite = $_SITE["backupdir"];

/* Which dates does the onsite have? */
print( "Extracting onsite backup dates from \"$onsite\"...\n" );
$result = `ls $onsite`;
$array = explode( "\n", $result );

/* Extract dates. */
for( reset( $array ); $filename = current( $array ); next( $array ) ) {
	print( "Considering \"$filename\"..." );

	if( preg_match( '/(\d+)\.(\d+)\.(\d+)\.tar\.gz/', $filename, $matches ) ) {
		print( " is compressed backup file.\n" );
		$datestr = $matches[1] . "." . $matches[2] . "." . $matches[3];
		$ondates[$datestr] = $datestr;
		}
	else if( preg_match( '/outfile\.[^.]*\.(\d+)\.(\d+)\.(\d+)/', $filename, $matches ) ) {
		print( " is an uncompressed backup file.\n" );
		$datestr = $matches[1] . "." . $matches[2] . "." . $matches[3];
		$ondates[$datestr] = $datestr;
	
		/* convert to tarball, if necessary */
		if( ! file_exists( $onsite . $datestr . ".tar.gz" ) ) {
//			print( "Compressing $datestr files...\n" );
			system( "cd $onsite; tar -z -c -f $datestr.tar.gz outfile.topics.$datestr outfile.users.$datestr" );
			system( "cd $onsite; rm outfile.*.$datestr" );
			}
		}
	else {
		print( " ignoring uknown filetype.\n" );
		}
	} /* end date extraction. */
?>
