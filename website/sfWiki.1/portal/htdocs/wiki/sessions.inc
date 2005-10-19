<?php
	/* Make sure we're only included once. */
	if( ! defined( "sfWikiSession" ) ) {
		define( "sfWikiSession", true );
		ini_alter( "session.use_cookies", true );
		ini_alter( "session.use_trans_sid", false );

		include( $_SITE["mirrordir"] . "database.inc" );
		
		/* declare the session-handling functions */
		function sessionOpen( $savePath, $sessionName ) { return true; }
		function sessionClose() { return true; }

		function sessionRead( $session_id ) {
			global $_SITE;

			$session_id = addslashes( $session_id );
			$result = wikiQuery( "SELECT session_id,session_date,session_data FROM " .
				$_SITE["dbsessiontable"] . " WHERE session_id=\"$session_id\";" );
			if( $result ) {
				$row = mysql_fetch_array($result);
				return $row["session_data"];
				} else {
				return false;
				}
			} /* end sessionRead() */

		function sessionWrite( $session_id, $session_data ) {
			global $_SITE;

			$session_id = addslashes( $session_id );
			$session_data = addslashes( $session_data );

			$result = wikiQuery( "REPLACE INTO " .
					$_SITE["dbsessiontable"] .
					" (session_id, session_date, session_data) VALUES" .
					" (\"$session_id\", UNIX_TIMESTAMP(), \"$session_data\");" );

			return $result;
			} /* end sessionRead() */
		
		function sessionDestroy( $session_id ) {
			global $_SITE;

			$session_id = addslashes( $session_id );
			$result = wikiQuery( "DELETE FROM " . $_SITE["dbsessiontable"] . 
					     " WHERE session_id=\"$session_id\";" );	

			return $result;
			} /* end sessionDestroy() */

		function sessionGarbageCollector( $sessionLifeTime ) {
			global $_SITE;

			$result = wikiQuery( "DELETE FROM " . $_SITE["dbsessiontable"] .
					     " WHERE session_date < ( UNIX_TIMESTAMP() - $sessionLifeTime );" );

			return $result;
			}

		function sessionSave() {
			sessionWrite( session_id(), session_encode() );
			}

		session_set_save_handler( 'sessionOpen', 'sessionClose', 'sessionRead', 'sessionWrite', 'sessionDestroy', 'sessionGarbageCollector' );
		} /* end if we've already been included. */

session_start();

?>
