<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );
require($_SITE["mirrordir"] . "web_util.inc");

/* display search results? */
if( ! isset( $query ) ) {
	$_PORTAL["title"] = "Searching";

	/* header */
	include($_SITE["portaldir"] . "CommonHeader.ihtml");
	include($_SITE["portaldir"] . "StaticHeader.ihtml");
	include($_SITE["mirrordir"] . "SearchBody.ihtml");
	}
else {
	$_PORTAL["title"] = "Results of search \"" . $query . "\"";

	/* header */
	include($_SITE["portaldir"] . "CommonHeader.ihtml");
	include($_SITE["portaldir"] . "StaticHeader.ihtml");
	include($_SITE["mirrordir"] . "search.inc" );
	printSearchResults( $query );
	}

/* footer */
require($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
