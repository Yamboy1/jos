#!/usr/bin/php -q
<?php

/* configuration */
$offsite = "/working/JJOS.d/joswiki-backup/";
$onsite = "/home/groups/j/jo/jos/sfWiki/portal/BACKUP/";
$target = "tmiller@65.0.206.139";

/* Which dates does the onsite have? */
print( "Extracting onsite backup dates...\n" );
$result = `ls $onsite`;
$array = explode( "\n", $result );

/* Extract dates. */
for( reset( $array ); $filename = current( $array ); next( $array ) ) {
	if( preg_match( '#(.*/)*(.*)\.tar\.gz#', $filename, $matches ) ) {
		$ondates[$matches[2]] = $matches[2];
		}
	else if( preg_match( '#outfile\..*\.(\d+)\.(\d+)\.(\d+)#', $filename, $matches ) ) {
		$datestr = $matches[1] . "." . $matches[2] . "." . $matches[3];
		$ondates[$datestr] = $datestr;
	
		/* convert to tarball, if necessary */
		if( ! file_exists( $onsite . $datestr . ".tar.gz" ) ) {
			print( "Compressing $datestr files...\n" );
			system( "cd $onsite; tar -z -c -f $datestr.tar.gz outfile.topics.$datestr outfile.users.$datestr" );
			}
		}
	} /* end date extraction. */

/* Which dates does the offsite already have? */
print( "Extracting offsite backup dates...\n" );
$result = `ssh -l tmiller 65.0.206.139 'ls $offsite'`;
$array = explode( "\n", $result );

/* Loop over the dates. */
for( reset( $array ); $filename = current( $array ); next( $array ) ) {
	preg_match( '#(.*/)*(.*)\.tar\.gz#', $filename, $matches );
	$offdates[$matches[2]] = $matches[2];
	}

if( sizeof( $offdates ) == 0 ) {
	$array = $ondates;
	}
else if ( sizeof( $ondates ) == 0 ) {
	die( "Unable to find local backup( date)s.\n" );
	}
else {
	/* Compare the two sets of dates. */
	$array = array_diff( $ondates, $offdates );
	}

/* Copy the disjoint dates. */
for( reset( $array ); $diffdate = current( $array ); next( $array ) ) {
	print( "Copying $diffdate.tar.gz offsite...\n" );
	passthru( "cd $onsite; scp ./$diffdate.tar.gz $target:$offsite" );
	}
?>
