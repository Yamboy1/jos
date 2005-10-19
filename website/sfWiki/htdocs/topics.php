<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

if( isset( $web ) && $web != "" ) {
	$addweb = " in $web";
	} else { $addweb = ""; }
$_PORTAL["title"] = "topics" . $addweb;

/* header */
require($_SITE["mirrordir"] . "web_util.inc");
require($_SITE["portaldir"] . "CommonHeader.ihtml");
require($_SITE["portaldir"] . "StaticHeader.ihtml");

/* verify input */
if( ! isset( $offset ) || $offset == "" ) {
	$offset = 0;
	}

/* body */
include($_SITE["mirrordir"] . "TopicBody.ihtml");

/* footer */
require($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
