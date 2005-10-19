<?php
function isOrphan( $topic ) {
	global $_SITE;

	$topicsl = addslashes( $topic );
        /* fetch the list of links */
        $result = wikiQuery( "SELECT topic,ltcount FROM " . 
		$_SITE["dbtable"] . " WHERE topic = \"$topicsl\";" );

	if( ! $result ) { return true; }
	$row = mysql_fetch_array( $result );
	if( $row["ltcount"] <= 0 ) {
 		return true;
		}

	return false;
	} /* end isOrphan() */
?>
