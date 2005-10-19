<?php
function checkRendering($topic, $web, $expected) {
	global $_SITE;

	/* Find the most recent of the four formatting files. */
	$chtime = filemtime( $_SITE["portaldir"] . "CommonHeader.ihtml" );
	$cftime = filemtime( $_SITE["portaldir"] . "CommonFooter.ihtml" );
	$shtime = filemtime( $_SITE["portaldir"] . "StaticHeader.ihtml" );
	$sftime = filemtime( $_SITE["portaldir"] . "StaticFooter.ihtml" );
 	$fmttime = max( $chtime, $cftime, $shtime, $sftime );
	$fmttime = strftime( "%Y%m%d%H%M%S", $fmttime );
	
	$result = FetchWikiEntry( $topic, $web, $expected );
	$row = mysql_fetch_array( $result );
	$time = $row["tstamp"];

	if( $time < $fmttime ) {
		if( ! function_exists( "saveWikiTopic" ) ) {
			include( $_SITE["mirrordir"] . "edit.inc" );
			}
		saveWikiTopic( $topic, $web, $row["full_text"], $row["summary"], false );
		return true;
		}

	return false;
	} /* end checkRendering */

function PrintWikiTopic($topic, $web, $expected = true)
{
if( $result = FetchWikiEntry($topic, $web, $expected) ) {
	/* only one row because topic is a primary key, and thus, unique. */
	$row = mysql_fetch_array($result);

	if ( checkRendering( $topic, $web, $expected ) ) {
		$result = FetchWikiEntry( $topic, $web, $expected );
		$row = mysql_fetch_array( $result );
		}

	print( stripslashes($row["rendertext"]) );
	} /* end if query succeeded */
} /* end PrintWikiTopic() */

function PrintWikiBodyText($topic, $web, $expected = true)
{
if( $result = FetchWikiEntry($topic, $web, $expected) ) {
	/* only one row because topic is a primary key, and thus, unique. */
	$row = mysql_fetch_array($result);
	$text = stripslashes($row["full_text"]);

	/* escape entities */
	// $text = preg_replace( "/&([^;]*);/", "&amp;\\1;", $text );
	print htmlentities($text);
	} /* end if query succeeded */
} /* end PrintWikiBodyText() */

function PrintWikiSummary($topic, $web, $expected = false)
{
if( ($result = FetchWikiEntry($topic, $web, false)) ) {
	$row = mysql_fetch_array($result);
	print( stripslashes($row["summary"]) );
	} 
} /* end printWikiSummary() */

function FetchWikiEntry($topic, $web, $expected)
{
global $_SITE;

/* does the web in question exist? */
if( ! webExists($web) ) {
	if ( $expected ) {
	        $editlink = $_SITE["docrootdir"] . "edit.php?web=" . $web . "&topic=" . urlencode($topic);
		include( $_SITE["mirrordir"] . "no_web.ihtml" );
		}
	return false;	
	}

$websl = addslashes($web);
$topicsl = addslashes($topic);

$query = "SELECT * FROM " . $_SITE["dbtable"] . " WHERE topic=\"$topicsl\"&&web=\"$websl\";";

/* perform a query */
if ( ! ($result = wikiQuery( $query ) ) ) {
	if ( $expected ) {
		$error_body = mysql_error() . " (" . mysql_errno() . ")";
		include( $_SITE["mirrordir"] . "query_failed.ihtml" );
		}
	return false;
	}	  

/* process the result */
if ( mysql_num_rows($result) == 0 ) {
	if ( $expected ) {
	        $editlink = $_SITE["docrootdir"] . "edit.php?web=" . $web . "&topic=" . urlencode($topic);
       		include($_SITE["mirrordir"] . "no_topic.ihtml");
		}
        return false;
	}
else {
	/* the topic body */
	return $result;
	}

return true;
} /* end fetchWikiEntry() */

?>
