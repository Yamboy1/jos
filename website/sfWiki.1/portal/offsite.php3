#!/usr/bin/php -q
<?php
include( "../site.inc" );

/* configuration */
$onsite = $_SITE["backupdir"];
$offsite = $_SITE["offsite_dir"];
$offsite_user = $_SITE["offsite_user"];
$offsite_address = $_SITE["offsite_address"];
$target = $offsite_user . "@" . $offsite_address;
if( $_SITE["offsite_port"] != "" ) {
	$ssh_port = "-p " . $_SITE["offsite_port"];
	$scp_port = "-P " . $_SITE["offsite_port"];
	} else {
	$ssh_port = "";
	$scp_port = "";
	}

/* Which dates does the onsite have? */
print( "Extracting onsite backup dates...\n" );
$result = `ls $onsite`;
$array = explode( "\n", $result );

/* Extract dates. */
for( reset( $array ); $filename = current( $array ); next( $array ) ) {
	if( preg_match( '#(.*/)*(.*)\.tar\.gz#', $filename, $matches ) ) {
		$ondates[$matches[2]] = $matches[2];
		}
	else if( preg_match( '#outfile\..*\.(\d+)\.(\d+)\.(\d+).(\d+)#', $filename, $matches ) ) {
		$datestr = $matches[1] . "." . $matches[2] . "." . $matches[3] . "." . $matches[4];
		$ondates[$datestr] = $datestr;
	
		/* convert to tarball, if necessary */
		if( ! file_exists( $onsite . $datestr . ".tar.gz" ) ) {
			print( "Compressing $datestr files...\n" );
			system( "cd $onsite; tar -z -c -f $datestr.tar.gz outfile.topics.$datestr outfile.users.$datestr" );
			system( "cd $onsite; rm outfile.*.$datestr" );
			}
		}
	} /* end date extraction. */

/* Which dates does the offsite already have? */
print( "Extracting offsite backup dates...\n" );
$result = `ssh -l $offsite_user $ssh_port $offsite_address 'ls $offsite'`;
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
	passthru( "cd $onsite; scp $scp_port ./$diffdate.tar.gz $target:$offsite" );
	}
?>
