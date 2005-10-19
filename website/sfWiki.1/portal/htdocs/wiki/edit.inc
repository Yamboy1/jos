<?php

require( $_SITE["mirrordir"] . "renderHTML.inc" );

function linkify( $value, $text ) {
	global $_SITE;

	$text = preg_replace( "/(\s)(.*)(\s)/U", "&nbsp;\\2&nbsp;", $text );

	if( preg_match( "@^cvs://@", $value ) ) { 
		/* cvs links */
		$value = preg_replace( "@cvs://([A-Za-z]+)/([^\s\.,!\?;:]+/)+([^\.\s\\,\!\?\;\;]+)((\.([^\.\s\,\!\?\:\:]+))*)@e",
			'returnContentType( 
			"<a href=\"" . "http://" . 
			 $_SITE["cvs_link"] . "\\1/\\2\\3\\4",
			"\\6",
			"\">$text</a>"
			);',
			$value );

		return $value;
		} /* end if cvs link */
	else if( preg_match( "@(.*)\://(.*)@U", $value ) ) {
		$value = preg_replace( "@(.*)\://(.*)@",
				"<a href=\"\\1://\\2\">$text</a>",
				$value );

		return $value;
		}
	
	return "<a href=\"$value\">$text</a>";
	} /* end linkif() */ 

function clip( $text ) {
	return preg_replace( '/\05/', '', $text );
	}

function returnContentType( $a, $ctype, $b ) {
	global $_SITE;

	if( $_SITE["cvs_ctype_" . $ctype] != "" ) {
		return clip($a) . "?content-type=" . $_SITE["cvs_ctype_" . $ctype] . $b;
		} else {
		return clip($a) . "?content-type=" . $_SITE["cvs_ctype_default"] . $b;
		}
	}

/* Shamelessly stolen from TWiki. */
function renderWikiText( $text, $topic, $web ) {
	global $_SITE;

	/* Handle in-line PREs by making them out-of-line. */
	$text = preg_replace( "/\<(\/?PRE)\>/i", "\n<\\1>\n", $text );
	$lines = split( "\n", $text );

	/* Prepare for rendering loop. */
	reset($lines);
	$insidePRE = false;
	$insideTABLE = false;

	$inList = false;
	$listDepth = 0;
	$listEnd = "";

	/* Rendering loop. */
	while( list( $key, $value ) = each($lines) ) {
		if( preg_match( "/<PRE>/i", $value ) ) { $insidePRE = true; }
		if( preg_match( "@</PRE>@i", $value ) ) { $insidePRE = false; }

		if( $insidePRE ) {
			/* Allow HTML entities, but replace
			   HTML delimiters with their entities. */
			$value = preg_replace( "/<PRE>/i", "$TT" . "PRE" . $TT, $value );
			$value = preg_replace( "/&(\w+?);/", "$TT\\1;", $value );
			$value = preg_replace( "/&(\#[0-9]+);/", "$TT\\1;", $value );
 
			$value = preg_replace( "/&/", "&amp;", $value );
			$value = preg_replace( "/</", "&lt;", $value );
			$value = preg_replace( "/>/", "&gt;", $value );
 
			$value = preg_replace( "/" . $TT . "PRE" . $TT . "/", "<PRE>", $value );
			$value = preg_replace( "/$TT/", "&", $value );
			} /* end if insidePRE */
		else {
			$TT = "\263";

			$value = preg_replace( "/\<(\/?LINK)\>/i", "%", $value );

			/* Mark legal HTML by replacing its delimiters with $TT.  */
			$value = preg_replace( "/\<(\/?H[123456789])\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?CODE)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?CITE)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?KBD)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?NOLINK)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?PRE)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?Q)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?SUB)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?SUP)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?S)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?STRIKE)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?TT)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?VAR)\>/i", "$TT\\1$TT", $value );

			$value = preg_replace( "/\<(\/?I)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?B)\>/i", "$TT\\1$TT", $value );
			$value = preg_replace( "/\<(\/?EM)\>/i", "$TT\\1$TT", $value );

			/* Change HTML delimiters into entities. */
			$value = preg_replace( "/</", "&lt;", $value );
			$value = preg_replace( "/>/", "&gt;", $value );

			/* Change legal HTML back into HTML. */
			$value = preg_replace( "/$TT(\/?H[123456789])$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?CODE)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?CITE)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?KBD)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?NOLINK)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?PRE)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?Q)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?SUB)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?SUP)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?S)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?STRIKE)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?TT)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?VAR)$TT/i", "<\\1>", $value );

			$value = preg_replace( "/$TT(\/?I)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?B)$TT/i", "<\\1>", $value );
			$value = preg_replace( "/$TT(\/?EM)$TT/i", "<\\1>", $value );

			/* Blockquote */
			$value = preg_replace( "/^>(.*?)$/", "&gt;<cite>\\1</cite><BR>", $value );

			/* fancy links */
			$value = preg_replace( "/%(.*)--&gt;(.*)%/Ue",
				'linkify( "\\1", "\\2" );',
				$value );
			
			/* FIXME : embedded-URL IMAGE links */
			preg_match_all( 
				"@(^|[\-\*\s])((http|ftp|gopher|news|https)\:(\S+[^\s\.,!\?;:]))@",
				$value, $matches, PREG_SET_ORDER );

			reset($matches);
			while( $xprs = current($matches) ) {
				if( preg_match( "/\.(gif|jpg|jpeg|png)$/i", $xprs[2] ) ) {
					$tag = "img src=";
					$following = "";
					} else {
					$tag = "a href=";
					$following = $xprs[2] . "</a>";
					}
				$html = $xprs[1] . '<' . $tag . '"' . 
					$xprs[2] .
					'">' . $following;
				$value = str_replace( $xprs[0], $html, $value ); 
				next($matches);
				} /* end loop over URL links */

			$value = preg_replace(
				"@(^|[\-\*\s])((http|ftp|gopher|news|https)\:(\S+[^\s\.,!\?;:]))@",
				"\\1<a href=\"\\2\">\\2</a>", $value );

			/* cvs links */
			$value = preg_replace( "@(^|[\-\*\s])cvs://([A-Za-z]+)/([^\s\.,!\?;:]+/)+([^\.\s\\,\!\?\;\;]+)((\.([^\.\s\,\!\?\:\:]+))*)@e",
				'returnContentType( 
				"\\1<a href=\"" . "http://" . 
				 $_SITE["cvs_link"] . "\\2/\\3\\4\\5",
				"\\7",
				"\">cvs://\\2/\\3\\4\\5</a>"
				);',
				$value );
			
			/* entities */
			$value = preg_replace(
				"/&(\w+?);/", "$TT\\1;", $value );
			$value = preg_replace(
				"/&(\#[0-9]+);/", "$TT\\1;", $value );
			$value = preg_replace(
				"/&/", "&amp;", $value );
			$value = preg_replace(
				"/$TT/", "&", $value );

			$value = preg_replace(
				"/^----*/", "<HR>", $value );
			$value = preg_replace(
				"@^([a-zA-Z0-9]+)----*@",
				"<table width=\"100%\"><tr><td valign=\"bottom\"><h2>\\1</h2></td><td width=\"98%\" valign=\"middle\"><HR></td></tr></table>",
				$value );

			/* table of format "| cell | [ cell | ]*" */
			if( preg_match( "/^(\s*)\|.*\|(\s*)$/", $value ) ) {
				if( $insideTABLE ) {
					$value = preg_replace(
						"/^(\s*)\|(\s*)(.*)/",
						"\\1<TR><TD>\\2\\3",
						$value );
					}
				else {
					$value = preg_replace(
						"/^(\s*)\|(\s*)(.*)/",
						"\\1<table border=\"2\" cellspacing=\"2\" cellpadding=\"2\"><TR><TD>\\2\\3",
						$value );
					}	
				
				$value = preg_replace(
					"/\|(\s*)$/",
					"</TD></TR>\\1",
					$value );
				$value = preg_replace(
					"/\|/", "</TD><TD>", $value );
				$insideTABLE = true;
				}
			else if ( $insideTABLE ) {
				$value = "</TABLE> " . $value;
				$insideTABLE = false;
				}

			/* lists */
			if( preg_match( "/^\S+?/", $value ) && $inList == true ) {
				/* not a list item */
				$listDepth = 0;
				$inList = false;
				$value = $listEnd . $value;
				$listEnd = "";
				} $endOfList = true;

			/* Moving this to a function would be hazardous. */
			if( preg_match( "/^((?:(?:   )|\t)+)(\S+?):\s/", $value, $matches ) ) {
				/* definition list item */
				$inList = true;
				$endOfList = false;
				$listItemDepth = strlen( $matches[1] );
				if( preg_match( "/ +/", $matches[1] ) ) {
					$listItemDepth = $listItemDepth / 3;
					}

				$prepend = "";
				$indent = 0;
				while( $listItemDepth > $listDepth ) {
					$prepend = "<DL> " . $prepend;
					$listEnd = "</DL> " . $listEnd;
					$listDepth = $listDepth + 1;
					$indent = 1;
					}
				while( $listItemDepth < $listDepth ) {
					$closing = substr( $listEnd, 0, 6 );
					$listEnd = substr( $listEnd, 6 );
					$prepend = $closing . $prepend;
					$listDepth = $listDepth - 1;		
					$indent = -1;
					}
				
				/* Am I in the same kind of list? */
				$closing = substr( $listEnd, 0, 6 );
				if( $closing != "</DL> " ) {
					$prepend = $prepend . "<DL> ";
					$listEnd = "</DL> " . $listEnd;
					$listDepth = $listDepth + 1;
					}

				$value = preg_replace( "/^((?:(?:   )|\t)+)(\S+?):\s/",
					      $prepend . "<DT> \\2: <DD> ",	
					      $value );
				} /* end definition list item */

			if( preg_match( "/^((?:(?:   )|\t)+)\* /", $value, $matches ) ) {
				/* unorded list item */
				$inList = true;
				$endOfList = false;
				$listItemDepth = strlen( $matches[1] );
				if( preg_match( "/ +/", $matches[1] ) ) {
					$listItemDepth = $listItemDepth / 3;
					}

				$prepend = "";
				while( $listItemDepth > $listDepth ) {
					$prepend = "<UL> " . $prepend;
					$listEnd = "</UL> " . $listEnd;
					$listDepth = $listDepth + 1;
					}
				while( $listItemDepth < $listDepth ) {
					$closing = substr( $listEnd, 0, 6 );
					$listEnd = substr( $listEnd, 6 );
					$prepend = $closing . $prepend;
					$listDepth = $listDepth - 1;
					}

				/* Am I in the same kind of list? */
				$closing = substr( $listEnd, 0, 6 );
				if( $closing != "</UL> " ) {
					$prepend = $prepend . "<UL> ";
					$listEnd = "</UL> " . $listEnd;
					$listDepth = $listDepth + 1;
					}
				
				$value = preg_replace( "/^((?:(?:   )|\t)+)\* /",
					      $prepend . "<LI> ",	
					      $value );
				} /* end unordered list item */

			if( preg_match( "/^((?:(?:   )|\t)+)((# )|(\d+\.?))/", $value, $matches ) ) {
				/* ordered list item */
				$inList = true;
				$endOfList = false;
				$listItemDepth = strlen( $matches[1] );
				if( preg_match( "/ +/", $matches[1] ) ) {
					$listItemDepth = $listItemDepth / 3;
					}

				$prepend = "";
				while( $listItemDepth > $listDepth ) {
					$prepend = "<OL> " . $prepend;
					$listEnd = "</OL> " . $listEnd;
					$listDepth = $listDepth + 1;
					}
				while( $listItemDepth < $listDepth ) {
					$closing = substr( $listEnd, 0, 6 );
					$listEnd = substr( $listEnd, 6 );
					$prepend = $closing . $prepend;
					$listDepth = $listDepth - 1;
					}

				/* Am I in the same kind of list? */
				$closing = substr( $listEnd, 0, 6 );
				if( $closing != "</OL> " ) {
					$prepend = $prepend . "<OL> ";
					$listEnd = "</OL> " . $listEnd;
					$listDepth = $listDepth + 1;
					}
				
				$value = preg_replace( "/^((?:(?:   )|\t)+)((# )|(\d+\.?))/",
					      $prepend . "<LI> ",	
					      $value );
				} /* end ordered list item */

			if( $endOfList && $inList ) {
				/* I found no list items,
				   but I /was/ in a list. */
				$listDepth = 0;
				$inList = false;
				$value = $listEnd . $value;
				$listEnd = "";
				}				

			/* blank lines become paragraphs */
			$value = preg_replace( "/^(\s*)$/", "</P><P>", $value );

			/* mailto */
			$value = preg_replace( 
				"#(^|[\s\(])(?:mailto\:)*([a-zA-Z0-9\-\_\.]+@[a-zA-Z0-9\-\_\.]+)(?=[\s\)]|$)#",
				'\\1<a href="mailto:\\2">\\2</a>',
				$value );

			/* And finally, WikiLinks... */

			/* Unlink <nolink></nolink> */
			$value = preg_replace(
				"/<nolink>(.*?)<\/nolink>/i",
				"<nolink>$TT\\1$TT</nolink>", 
				$value );

			/* Web.TopicName */
			$value = preg_replace(
				"/(^|[\*\s][\(\-\*\s]*)([A-Z]+[a-z]*)\.([A-Z]+[a-z]+(?:[A-Z]+[a-zA-Z0-9]*))/",
				'\\1<a href="' . $_SITE["docrootdir"] .
				'view/\\2/\\3">' . $TT . "\\2.\\3" . $TT . '</a>' .
				"<wikitopic $TT\\3$TT /wikitopic>",
				$value );

			/* TopicName */
			$value = preg_replace(
				"/(^|[\*\s][\(\-\*\s]*)([A-Z]+[a-z]+(?:[A-Z]+[a-zA-Z0-9]*))/",
				'\\1<a href="' . $_SITE["docrootdir"] .
				"view/$web/\\2\">$TT\\2$TT</a>" .
				"<wikitopic $TT\\2$TT /wikitopic>",
				$value );
			
			/* Link acronyms. */
			$value = preg_replace(
				"/(^|[\*\s][\-\*\s]*)([A-Z][A-Z][A-Z]([A-Z]*[a-z]*))/",
				'\\1<a href="' . $_SITE["docrootdir"] .
				"view/$web/\\2\">$TT\\2$TT</a>" .
				"<wikitopic $TT\\2$TT /wikitopic>",
				$value );
		
			$value = preg_replace( "/\>" . $TT . "(.*?)" . $TT . "\</",
				">\\1<", $value );
			$value = preg_replace( "#\<wikitopic $TT(.*?)$TT /wikitopic>#", "<wikitopic \\1 /wikitopic>", $value );

			/* A few custom markups. */
		
			/* '*' and '_' as bold and underline, respectively. */
			$value = preg_replace(
				"/(^|\s)\*_([^\*_\s][^\*_]*[^\*_\s])_\*($|\s)/",
				"\\1<STRONG><EM>\\2</EM></STRONG>\\3",
				$value );
			$value = preg_replace(
				"/(^|\s)_\*([^\*_\s][^\*_]*[^\*_\s])\*_($|\s)/",
				"\\1<STRONG><EM>\\2</EM></STRONG>\\3",
				$value );
			$value = preg_replace(
				"/(^|\s)\*([^\*\s][^\*]*[^\*\s])\*($|\s)/",
				"\\1<STRONG>\\2</STRONG>\\3",
				$value );
			$value = preg_replace(
				"/(^|\s)_([^_\s][^_]*[^_\s])_($|\s)/",
				"\\1<EM>\\2</EM>\\3",
				$value );

			/* WikiLinks from JOSWiki... */

			/* %Web:WikiName% */
			$value = preg_replace(
				"/(^|\s|\()\%([^\s].*?[^\s])\:([^\s].*?[^\s])\%/",
				'\\1<a href="' . $_SITE["docrootdir"] .
				'view/\\2/\\3">\\2:\\3</a>' .
				'<wikitopic \\3 /wikitopic>',
				$value );

                        /* %Odd Wiki Name% */
                        $value = preg_replace(
                                "/(^|[^%])%(\w|(\w[^%]*\w))%([^%]|$)/",
                                '\\1<a href="' . $_SITE["docrootdir"] .
                                "view/$web/\\2\">\\2</a>" .
                                "<wikitopic \\2 /wikitopic>\\4",
                                $value );

                        $value = preg_replace(   
                                "/(^|\s)%(\w|(\w[^%]*\w))%($|\s)/",
                                '\\1<a href="' . $_SITE["docrootdir"] .
                                "view/$web/\\2\">\\2</a>" .
                                "<wikitopic \\2 /wikitopic>\\4",
                                $value );
			
			/* %Web.Odd Wiki Name% */
			$value = preg_replace(
				"/(^|\s|\()\%([a-zA-Z0-9]+)\.(.*?[^\s])\%(\s|\)|$)/",
				'\\1<a href="' . $_SITE["docrootdir"] .
				'view/\\2/\\3\">\\3</a>' .
				'<wikitopic \\3 /wikitopic>',
				$value );				
	
			/* Render inclusions. */
			$value = preg_replace( "@{cvs://([A-Za-z]+)/([^\s\.,!\?;:]+/)+([^\.\s\\,\!\?\;\;]+)((\.([^\.\s\,\!\?\:\:]+))*)}@e",
				'join( "", file(returnContentType( 
				"http://" . 
				$_SITE["cvs_link"] . "\\1/\\2\\3\\4",
				"\\6",
				""
				)))', $value );

			$value = preg_replace(
				"/{(.*):\/\/(.*)}/Ue",
				'join( "", file( "\\1://\\2" ) );',
				$value
				); 

			} /* end if not insidePRE */

		/* Store the rendering done so far. */
		$result = $result . "\n" . $value;
		} /* end rendering loop */
	
	/* Take care of some terminal conditions. */
	if( $insideTABLE ) { $result = $result . "</TABLE>\n"; }
	if( $insidePRE ) { $result = $result . "</PRE>\n"; }
	$result = $result . $listEnd;

	/* Fix up 'Topics With Spaces'. */
	$result = preg_replace( '/topic\=([^"]+)&web\=/e',
		'"topic=" . urlencode( "\\1" ) . "&web="',
		$result );
	$result = preg_replace( '/topic\=([^"&]+)\"/e',
		'"topic=" . urlencode( "\\1" ) . "\""',
		$result ); 

	/* Return the finished product. */
	return "<P>" . $result . "<BR><BR></P>";	// Formatting.
} /* end renderWikiText() */

function stripLinks( $render_text ) {
	preg_match_all( "/(\<wikitopic )([^<>]*)( \/wikitopic\>)/",  $render_text, $matches );
	return $matches[2];
} /* end stripLinks() */

function saveWikiTopic($topic, $web, $body_text, $summary, $adjustTime) {
global $_SITE;
global $username;

/* should we delete the page? */
if( preg_match( "/^\s*$/", $body_text ) && preg_match( "/^\s*$/", $summary ) ) { 
	wikiQuery( "DELETE FROM " . $_SITE["dbtable"] . " WHERE topic=\"$topic\";" );
	header( "Location: " . $_SITE["docrootdir"] . "view/$web/" . urlencode($topic) );
	return;
	}

/* generate the rendertext */
if( $web == "Imported" ) {
	$render_text = renderHTML( stripslashes($body_text), $topic, $web );
	} else {
	$render_text = renderWikiText( stripslashes($body_text), $topic, $web );
	}

/* generate the list of links from this page*/
$matches = stripLinks( $render_text );
if( sizeof($matches) == 0 ) {
	$links_from = "";
	$lfcount = 0;
	}
else {
	reset($matches);
	do {
		$lfarray[current($matches)] = current($matches);
		} while( next($matches) );
	$lfcount = sizeof($lfarray);
	$links_from = implode( "\n", $lfarray );
	$links_from = "\n" . $links_from . "\n";
	}

/* we must escape the quotes from our
   inputs.  Because PHP doesn't need the slashes,
   don't over-write the originals. */
$body_textsl = addslashes($body_text);
$render_textsl = addslashes($render_text);
$websl = addslashes($web);
$topicsl = addslashes($topic);
$summarysl = addslashes($summary);
$links_fromsl = addslashes($links_from);

/* Calling fetchNumber() breaks the database connection for some reason... */
$ltcount = fetchNumber( "SELECT COUNT(*) FROM " . $_SITE["dbtable"] . " WHERE " . 
                        "linksfrom REGEXP \"\n" . $topicsl . "\n\";" );

/* Update the node count. */
$oldlf = wikiQuery( "SELECT linksfrom,full_text FROM " . $_SITE["dbtable"] . " WHERE topic=\"" . $topic . "\";" );
$oldlff = mysql_fetch_array( $oldlf );
$old_full_textsl = addslashes($oldlff["full_text"]);
$old_links_from = $oldlff["linksfrom"];
$arrayx = split( "\n", $old_links_from );
for( $ii = 0; $ii < sizeof( $arrayx ); $ii++ ) {
	$linkfrom = $arrayx[$ii];
	if( $linkfrom == "" ) { continue; }
	if( $linkfrom == $topic ) { continue; }
	wikiQuery( "UPDATE " . $_SITE["dbtable"] . " SET ltcount = ltcount - 1 WHERE topic = \"" . $linkfrom . "\";" );
	}

/* perform the save qeury */
if( isset($adjustTime) && $adjustTime == true ) {
	$result = wikiQuery( "REPLACE INTO " . $_SITE["dbtable"] . " " .
		"(topic,web,summary,rendertext,full_text,old_full_text,linksfrom,lfcount,ltcount,lastedit,lasteditor) " .
		"VALUES (\"$topicsl\", \"$websl\", \"$summarysl\", \"$render_textsl\", \"$body_textsl\", \"$old_full_textsl\", \"$links_fromsl\", \"$lfcount\", \"$ltcount\", NOW(), \"$username\" );" );
	}
else {
	$result = wikiQuery( "UPDATE " . $_SITE["dbtable"] . " SET " .
			"topic = \"$topic\", " .
			"web = \"$web\", " .
			"summary = \"$summarysl\", " . 
			"full_text = \"$body_textsl\", " .
			"old_full_text = \"$old_full_textsl\", " .
			"linksfrom = \"$links_fromsl\", " .
			"rendertext = \"$render_textsl\", " .
			"ltcount = \"$ltcount\", " .
			"lfcount = \"$lfcount\" " .
			"WHERE topic = \"$topic\";" );
	}	

if( ! $result ) {
	$error_body = mysql_errno() . ": (edit.inc/503) " . mysql_error;
        include( $_SITE["mirrordir"] . "query_failed.ihtml" );
        return false;
        }

/* Update the node count. */
$arrayx = split( "\n", $links_from );
for( $ii = 0; $ii < sizeof($arrayx); $ii++ ) {
	$linkfrom = $arrayx[$ii];
	if( $linkfrom == "" ) { continue; }
	if( $linkfrom == $topic ) { continue; }
	wikiQuery( "UPDATE " . $_SITE["dbtable"] . " SET ltcount = ltcount + 1 WHERE topic = \"" . $linkfrom . "\";");
	}

return true;
} /* end saveWikiTopic() */

function fetchNumber( $query ) {
	global $_SITE;

	/* Why can't I pass database connection indices around?
	   I can't seem to reuse them up above, either... */
	$result = wikiQuery( $query );

	if( ! $result ) {
        	$error_body = mysql_errno() . ": (edit.inc/528) " . mysql_error();
	        include( $_SITE["mirrordir"] . "query_failed.ihtml" );
        	return false;
        	}
	
	$row = mysql_fetch_row($result);
	$count = $row[0];
	if( $count == "" ) { return 0; } else { return $count; }
} /* end fetchNumber() */
?>
