<?php

function printWikiTable( $result, $header, $context = false, $counts = false ) {
	global $_SITE;

	/* open table */
	print( "\n" . '<TABLE WIDTH="100%" BORDER="0" CELLSPACING="1" CELLPADDING="2">' . "\n" );
	
	/* column headings */
	print( '<TR BGCOLOR="' . $_SITE["darkbgcolor"] . '">' . "\n" );

	print( '<TD><B><FONT COLOR="' . $_SITE["textcolor"] . '">' . "\n" );
	print( $header . '</FONT></B></TD>' . "\n" );

	if( $counts ) {
		print( '<TD><B><FONT COLOR="' . $_SITE["textcolor"] . '">' . "\n" );
		print( 'views' . '</FONT></B></TD>' . "\n" );
		}

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
			"view//" . urlencode($row["topic"]) .
			"\">" . $row["topic"] . '</a>' );
		print( " <a href=\"" . $_SITE["docrootdir"] .
			"info/" . $row["web"] . "/" . urlencode($row["topic"]) .
			"\"><FONT SIZE=\"-1\">[info]</FONT></a>" );
		if( $row["ltcount"] <= 0 ) {
			print( ' (orphan)' );
			}
		print( '</TD>' );

		if( $counts ) {
			$current_mo_number = date( "m" );
			$total = 0;
			$topicsl = addslashes( $row["topic"] );
			$mo_result = wikiQuery( "SELECT * FROM " . $_SITE["dbstattable"] .
			        " WHERE topic = \"$topicsl\";" );
			$mo_row = mysql_fetch_array( $mo_result );
			
			print( '<TD ALIGN="CENTER">' . $mo_row[ sprintf("m%'02d", $current_mo_number) ] );
			for( $mo_number = $current_mo_number; $mo_number > $current_mo_number - 12; $mo_number-- ) {
			        if( $mo_number <= 0 ) { $use_mo_number = $mo_number + 12; }
		                else { $use_mo_number = $mo_number; }

			        $use_mo_name = sprintf( "m%'02d", $use_mo_number );
			        $total += $mo_row[$use_mo_name];
			        } /* end month printing loop. */
			print( "/$total" . '</TD>' );
			} /* end statistics */

		/* last edit date */
		$lastedit = $row["lastedit"];
		$lasteditor = $row["lasteditor"];
		if( $lastedit == "" ) {
			$lastText = '&nbsp;';
			} else {
			$lastText = $lastedit;
			if( $lasteditor != "" ) {
				$lastText = $lastText . ", by " . 
					'<a href="' . $_SITE["docrootdir"] .
					'view//' . $lasteditor . '">' . $lasteditor .
					'</a>';
				}
			}
		print( '<TD>' . $lastText . '</TD>' );

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
