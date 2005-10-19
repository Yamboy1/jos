<?php

require( $_SITE["mirrordir"] . "sessions.inc" );

/* register the session variables associated with user authentication. */
session_register( "username" );
session_register( "password" );
session_register( "email" );

function logout() {
	session_unset();
	session_destroy();
	}

function changeLogin( $postpwd, $postvfd, $postemail ) {
	global $_SITE, $username, $password;

	if( isset( $postpwd ) &&
	    isset( $postvfd ) &&
	    $postpwd != "" && $postpwd == $postvfd ) {
		/* Write $postpwd to the database for this account. */
		wikiQuery( "UPDATE " . $_SITE["dbusertable"] . 
			   " SET password=\"$postpwd\" WHERE username=\"$username\";" );
		$password = $postpwd;
		sessionSave();
		}
	if( isset( $postemail ) && $postemail != "" ) {
		/* Write $postemail to the database for this account. */
		wikiQuery( "UPDATE " . $_SITE["dbusertable"] . 
			   " SET email=\"$postemail\" WHERE username=\"$username\";" );
		}
	}

function mailPassword( $username, $email ) {
	global $_SITE, $SERVER_NAME;

	$result = wikiQuery( "SELECT * FROM " . $_SITE["dbusertable"] .
			     " WHERE username=\"$username\" OR email=\"$email\";" );
	if( $result && mysql_num_rows( $result ) > 0 ) {
		$array = mysql_fetch_array( $result );
		mail( $email, "Wiki at $SERVER_NAME", "\"" . $username . "\" has password \"" . $array["password"] . "\"." );
		return "";
		} else {
		return "Unable to locate user name or email.";
		}
	}



function createNewLogin( $username, $password, $email ) {
	global $_SITE;

	/* Verify that $username is a WikiName. */
	if ( ! preg_match( "/^([A-Z]+[a-z]*)([A-Z]+[a-z]*)+$/", $username ) ) {
		$return =  "\"$username\" is not a valid WikiName, please try again.";
		$return = $return . "\n<BR>A WikiName must start with a capital letter, consist of only letters, and contain at least one more capital letter.<BR>\n";
		return $return;
		}
	
	/* Verify that $username isn't already taken. */
	$result = wikiQuery( "SELECT * FROM " . $_SITE["dbusertable"] .
			     " WHERE username=\"$username\";" );
	if ( mysql_num_rows( $result ) == 1 ) {
		return "UserName \"$username\" is already taken, please try again.";
		}

	/* Verify that the e-mail address could be valid. */
	if( ! preg_match( "/[a-zA-Z0-9\-\_\.]+@[a-zA-Z0-9\-\_\.]+/", $email ) ) {
		return "\"$email\" is not a valid e-mail, please try again.";
		}

	/* Enter the new user into the database. */
	$result = wikiQuery( "REPLACE INTO " . $_SITE["dbusertable"] .
			     " (username, password, email) VALUES " .
			     " (\"$username\", \"$password\", \"$email\");" );

	return "";
	}

function isAuthenticated() {
	global $_SITE, $password, $username, $postpassword, $postusername;

	/* Is session information correct? */
	$result = wikiQuery( "SELECT * FROM " . $_SITE["dbusertable"] .
			     " WHERE username=\"$username\";" );
	if( $result && mysql_num_rows( $result) == 1 ) {
		$array = mysql_fetch_array( $result );
		if( $array["password"] == $password ) {
			return true;
			}
		}

	/* Is the POSTed form information correct? */
        $result = wikiQuery( "SELECT * FROM " . $_SITE["dbusertable"] .
                             " WHERE username=\"$postusername\";" );
        if( $result && mysql_num_rows( $result) == 1 ) {
                $array = mysql_fetch_array( $result );
                if( $array["password"] == $postpassword ) {
			$username = $postusername;
			$password = $postpassword;
                        return true;
                        }
                }                       

	return false;
	} /* end isAuthenticated() */

?>
