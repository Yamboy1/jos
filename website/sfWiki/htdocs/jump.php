<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

include( $_SITE["mirrordir"] . "web_util.inc" );
include( $_SITE["mirrordir"] . "view.inc" );

$web = verifyTopicWeb( $topic );

if( FetchWikiEntry( $topic, $web, false ) ) {
	header( "Location: " . $_SITE["docrootdir"] . "view/$web/" . urlencode($topic) );
	} else {
	header( "Location: " . $_SITE["docrootdir"] . "search.php?query=" . urlencode( $topic ) );
	}
?>
