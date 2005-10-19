<?php

function printWikiTable( $result, $header, $context = false ) {
	global $_SITE;

	/* open table */
	print( "\n" . '<TABLE WIDTH="100%" BORDER="0" CELLSPACING="1" CELLPADDING="2">' . "\n" );
	
	/* column headings */
	print( '<TR BGCOLOR="' . $_SITE["darkbgcolor"] . '">' . "\n" );

	print( '<TD><B><FONT COLOR="' . $_SITE["textcolor"] . '">' . "\n" );
	print( $header . '</FONT></B></TD>' . "\n" );

	print( '<TD><B><FONT COLOR="' . $_SITE["textcolor"] . '">' . "\n" );
	print( 'last edit</FONT></B></TD>' . "\n" );

	print( '<TD><B><FONT COLOR="' . $_SITE["textcolor"] . '">' . "\n" );
	print( 'summary</FONT></B></TD>' . "\n" );
	
	print( '</TR>' );

	while( $row = mysql_fetch_array( $result ) ) {
		/* alternate white & light background color */
		if( $alter = !$alter ) {
			print( '<TR BGCOLOR="#FFFFFF">' );
			} else {
			print( '<TR BGCOLOR="' . $_SITE["lightbgcolor"] . '">' );
			}

		/* topic link */
		print( "<TD><a href=\"" . $_SITE["docrootdir"] .
			"view.php?topic=" . urlencode($row["topic"]) .
			"&web=" . $row["web"] . "\">" . $row["topic"] . '</a>' );
		if( $row["ltcount"] <= 0 ) {
			print( ' (orphan)' );
			}
		print( '</TD>' );

		/* last edit date */
		$lastedit = $row["lastedit"];
		if( $lastedit == "" ) {
			print( '<TD>&nbsp;</TD>' );
			} else {
			print( '<TD>' . $row["lastedit"] . '</TD>' );
			}

		/* summary */
		$summary = stripslashes($row["summary"]);
		if( $summary == "" ) { 
			print( '<TD>&nbsp;</TD>' );
			} else {
			print( '<TD>' . $summary . '</TD>' );
			}

		/* end of row */
		print( "</TR>\n" );

		if( $context ) {
			if( $alter == true ) {
				print( '<TR BGCOLOR="#FFFFFF">' );
				} else {
				print( '<TR BGCOLOR="' . $_SITE["lightbgcolor"] . '">' );
				}
			print( '<TD COLSPAN="1"></TD><TD COLSPAN="2"><B>context:</B> ' );
			print( $context[ $row["topic"] ] );
			print( '</TD></TR>' );
			} /* end context */		
	} /* end table rows iteration */
	print( '</TABLE>' );
} /* end printWikiTable() */

?>
