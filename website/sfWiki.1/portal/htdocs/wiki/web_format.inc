<?php
function fetchWebHeader($web) {
	if ( $result = FetchFormatEntry( "[format-$web-header]" ) ) {
        	/* only one row because topic is a primary key, and thus, unique. */
        	$row = mysql_fetch_array($result);

		return stripslashes($row["full_text"]);
		} /* end if query suceeded */
        } /* end printWebHeader() */
  
function fetchWebfooter($web) {
	if ( $result = FetchFormatEntry( "[format-$web-footer]" ) ) {
        	/* only one row because topic is a primary key, and thus, unique. */
        	$row = mysql_fetch_array($result);
		
		return stripslashes($row["full_text"]);
		} /* end if query suceeded */
        }
?>
