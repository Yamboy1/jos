<?php
require( "../site.inc" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

/* includes */
require($_SITE["mirrordir"] . "view.inc" );
require($_SITE["mirrordir"] . "web_util.inc" );
require($_SITE["mirrordir"] . "web_format.inc" );

/* do we have something to look for? */
if (!isset($topic) || $topic == "")
{
  $topic = $web . "WebHome";
}

/* Select the correct web. */
$web = verifyTopicWeb( $topic, $web );

/* header */
require($_SITE["portaldir"] . "CommonHeader.ihtml");
require($_SITE["portaldir"] . "StaticHeader.ihtml");

/* body */
PrintWikiTopic($topic,$web,true);

/* footers */
require($_SITE["mirrordir"] . "ViewFooter.ihtml");
require($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
