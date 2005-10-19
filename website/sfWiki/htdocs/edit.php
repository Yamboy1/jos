<?php
require( "../site.inc" );

// session_cache_limiter( "public, must-revalidate" );
include( $_SITE["mirrordir"] . "sessions.inc" );
include( $_SITE["mirrordir"] . "userauth.inc" );

/* Verify that the user is authorized [to edit this page]. */
if( ! isAuthenticated() ) {
	/* redirect the user to login. */
	header( "Location: " . $_SITE["docrootdir"] . "user.php?op=edit&redirect=" . urlencode($REQUEST_URI) );
	}

/* includes */
include($_SITE["mirrordir"] . "view.inc");
include($_SITE["mirrordir"] . "web_util.inc" );
include($_SITE["mirrordir"] . "web_format.inc" );

/* verify our inputs */
if (!isset($topic) || $topic == "")   
{
	include($_SITE["portaldir"] . "CommonHeader.ihtml");
	include($_SITE["portaldir"] . "StaticHeader.ihtml");
	print( "Which topic did you want to edit?<BR>" );
	include($_SITE["portaldir"] . "StaticFooter.ihtml");
	include($_SITE["portaldir"] . "CommonFooter.ihtml");
	exit;	  
}

/* Migrate to the correct web. */
$webV = verifyTopicWeb( $topic, $web );

/* did we change the web? */
if( $webV != $web && ( isset($save) || isset($preview) ) ) {
	; } else { $web = $webV; }

if( $web == "Imported" && ( isset($save) || isset($preview) ) ) {
	print( "<H2>You can't save to the Import web, please select another.</H2>\n" );
	print( "<h3>The following preview assumes you've save to some other web. (Which disables most HTML; see the <a href=\"/view//WikiFormat\">WikiFormat</a>ting rules.)</H3>\n" );
	unset( $save ); $preview = "";
	}


if (isset($save)) {
	$_PORTAL["title"] = "Saved [" . $topic . "]";

	include($_SITE["mirrordir"] . "edit.inc");
	/* save the changes, then ... */
	if (saveWikiTopic($topic, $web, $body_text, $summary, true)) {
		header( "Location: view.php?topic=" . urlencode($topic) . "&web=$web" );
		exit();
		} else {
		/* the error has already been handled */
		}
	}

/* headers */
require($_SITE["portaldir"] . "CommonHeader.ihtml");
include($_SITE["portaldir"] . "StaticHeader.ihtml");

/* body */ 	

/* EditBody.ihtml defines $save, $preview, and $body_text
   in its forms, which have this script as their actions. */
if (isset($preview)) {
	$_PORTAL["title"] = "Previewing [" . $topic . "]";

	include($_SITE["mirrordir"] . "edit.inc");
	/* preview the changes */
	include($_SITE["mirrordir"] . "PreviewHeader.ihtml" );
	print( renderWikiText( stripslashes($body_text), $topic, $web ) );
	include($_SITE["mirrordir"] . "PreviewFooter.ihtml" );

	/* allow more editing */
	include($_SITE["mirrordir"] . "EditBody.ihtml" );
	}
else if (isset($rollback)) {
	$_PORTAL["title"] = "Editing [" . $topic . "]";

	$result = fetchWikiEntry( $topic, $web, false );
	if( $result ) {
		/* If there's nothing to rollback to, ignore it. */
		$row = mysql_fetch_array( $result );
		$body_text = $row["old_full_text"];
		}

	include($_SITE["mirrordir"] . "EditBody.ihtml" );
	}	
else {
	$_PORTAL["title"] = "Editing [" . $topic . "]";

	include($_SITE["mirrordir"] . "EditBody.ihtml" );
	}

/* footers */
include($_SITE["portaldir"] . "StaticFooter.ihtml");
require($_SITE["portaldir"] . "CommonFooter.ihtml");
?>
