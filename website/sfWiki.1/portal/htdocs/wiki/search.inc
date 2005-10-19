<?php
include( $_SITE["mirrordir"] . "table.inc" );

function printSearchResults( $query ) {
	global $_SITE;

	if( ! preg_match( '/^[^\"]+$/', $query ) ) {
		/* Hope this catches enough. */
		print( "Invalid search string: please don't use quotes.  Aborting.\n" );
		return;
		}

	$text = addslashes( $query );
	/* Search is prioritized by topic, summary, keywords, full_text,
	   so do them in order; may add interface to query these
	   individually. */
	$ordering["topic"] = "the Wiki Topic";
	$ordering["summary"] = "the summary of";
	$ordering["full_text"] = "the full text of";

	$result_count = 0;
	print( '<H3>the search found "<TT>' . $query . '</TT>" in</H3>' . "\n" );
	for( reset($ordering); $key = key($ordering); next($ordering) ) {
		if( $key == "full_text" ) {
			/* Include context information. */
			$result = wikiQuery( "SELECT topic,full_text FROM " . $_SITE["dbtable"] . " WHERE full_text LIKE \"%$text%\" ORDER BY lastedit DESC;" );

			if( !$result || mysql_num_rows( $result ) == 0 ) {
				continue;
				}

			while( $row = mysql_fetch_array( $result ) ) {
				/* change query from SQL to PERL */
				$query = preg_replace( '/\%/', '.*', $query );
				$query = preg_replace( '/\_/', '.', $query );
				/* clean up the full text string. */
				$ft = stripslashes( $row["full_text"] );
				/* bold what we found; use \263 to avoid de-HTMLization following. */
				$ft = preg_replace( "%($query)%i", "\263\\1\263", $ft );
				/* use up to two lines surrounding the first result in either direction. */
				preg_match( '%(.*\n)?(.*\n)?.*' . $query . '.*(\n.*)?(\n.*)?%mi', $ft, $matches );
				$str = $matches[0];			

				/* Change HTML delimiters into entities. */
				$str = preg_replace( "/</", "&lt;", $str );
				$str = preg_replace( "/>/", "&gt;", $str );

				/* fix bolding. */
				$str = preg_replace( "%\263($query)\263%i", "<B>\\1</B>", $str );

				$context[$row["topic"]] = $str;
				}
			
			$result = wikiQuery( "SELECT topic,web,summary,lastedit,ltcount FROM " . $_SITE["dbtable"] . " WHERE $key LIKE \"%$text%\" ORDER BY lastedit DESC;" );
			printWikiTable( $result, $ordering[$key], $context );
			$result_count++;
			continue;
			} /* end context loop. *
			
		/* fetch the results */
		$result = wikiQuery( "SELECT topic,web,summary,lastedit,ltcount FROM " . $_SITE["dbtable"] . " WHERE $key LIKE \"%$text%\" ORDER BY lastedit DESC;" );

		/* print them. */
		if( !$result || mysql_num_rows( $result ) == 0 ) {
			continue;
			} else {
			printWikiTable( $result, $ordering[$key] );
			print( '<BR>' );
			$result_count++;
			}
                } /* end query loop */
                        
        if ( $result_count == 0 ) {
                print( "<H4>nothing.</H4>" );
		}
	} /* end printSearchResults */
?>
