<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

require($_SITE["mirrordir"] . "web_util.inc");
require($_SITE["portaldir"] . "CommonHeader.ihtml");

/* verify input */
if (!isset($topic) || $topic == "") {
	/* header */
	include($_SITE["portaldir"] . "StaticHeader.ihtml");
	/* body */
	print( "Which topic did you want to get information on?<BR>" );
        } else {
        $web = verifyTopicWeb( $topic, $web );
	$_PORTAL["title"] = "Information on [" . $topic . "]";
	/* header */
	include($_SITE["portaldir"] . "StaticHeader.ihtml");
	/* body */
        include($_SITE["mirrordir"] . "InfoBody.ihtml");
	print( "<BR>" );
        }

/* footer */
require($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
