<?php
function PrintWikiTopic($topic, $web, $expected = true)
{
global $_SITE;

if( $result = FetchWikiEntry($topic, $web, $expected) ) {
	/* only one row because topic is a primary key, and thus, unique. */
	$row = mysql_fetch_array($result);

	/* Increment view count. */
	$topicsl = addslashes( $topic );
	$month = "m" . date( "m" );
	wikiQuery( "UPDATE " . $_SITE["dbstattable"] . " SET $month = $month + 1 WHERE topic = \"$topicsl\";" );
	if( mysql_affected_rows() == 0 ) {
		wikiQuery( "REPLACE INTO " . $_SITE["dbstattable"] . " (topic) VALUES ( \"$topicsl\" );" );
		wikiQuery( "UPDATE " . $_SITE["dbstattable"] . " SET $month = $month + 1 WHERE topic = \"$topicsl\";" );
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
	        $editlink = $_SITE["docrootdir"] . "edit/" . $web . "/" . urlencode($topic);
		include( $_SITE["mirrordir"] . "no_web.ihtml" );
		}
	return false;	
	}

$websl = addslashes($web);
$topicsl = addslashes($topic);

$query = "SELECT * FROM " . $_SITE["dbtable"] . " WHERE topic = \"$topicsl\" && web=\"$websl\";";

/* perform a query */
if ( ! ($result = wikiQuery( $query ) ) ) {
	if ( $expected ) {
		$error_body = mysql_errno() . ": (view.inc)" . mysql_error();
		include( $_SITE["mirrordir"] . "query_failed.ihtml" );
		}
	return false;
	}	  

/* process the result */
if ( mysql_num_rows($result) == 0 ) {
	if ( $expected ) {
	        $editlink = $_SITE["docrootdir"] . "edit/" . $web . "/" . urlencode($topic);
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
