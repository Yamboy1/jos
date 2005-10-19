<?php
include( $_SITE["mirrordir"] . "database.inc" );

function getWebList() {
	global $_SITE;
	$webs = array();

	if ( ! $result = wikiQuery( "SELECT DISTINCT web FROM " . $_SITE["dbtable"] . ";" ) ) {
		$webs["Main"] = "Main";
		return $webs;
		}

	while( $row = mysql_fetch_row($result) ) {
		$webs[$row[0]] = $row[0];
		}
	return $webs;
	} /* getWebList() */

function webExists( $web ) {
	if( ! $webs = getWebList() ) { return false; }
 	for ( reset($webs); $key = key($webs); next($webs) ) {
		if( $key == $web ) { return true; }
		}
	return false;
	} /* end webExists() */

function verifyTopicWeb( $topic, $web="Main" ) {
	global $_SITE;

	/* prepare the topic */
	$topicsl = addslashes($topic);
	
	/* make the query */
	if( ! $result = wikiQuery( "SELECT web FROM " . $_SITE["dbtable"] . " WHERE topic = \"$topicsl\";" ) ) { return "Main"; }

	/* process the result */
	if( mysql_num_rows( $result ) != 1 ) {
		if( isset( $web ) && $web != "" ) {
			return $web;
			} else {
			return "Main";
			}
		}
	$row = mysql_fetch_row( $result );
	return $row[0];
	} /* end verifyTopicWeb() */
?>
