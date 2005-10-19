<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

$_PORTAL["title"] = "the Wiki";

/* header */
require($_SITE["mirrordir"] . "web_util.inc");
require($_SITE["portaldir"] . "CommonHeader.ihtml");
require($_SITE["portaldir"] . "StaticHeader.ihtml");

/* body */
require($_SITE["mirrordir"] . "IndexBody.ihtml");

/* footer */
require($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
