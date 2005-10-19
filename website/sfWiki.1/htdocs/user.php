<?php
	/* save the POSTed variables, in case we want them later. */
	reset( $HTTP_POST_VARS );
	while ( list ( $key, $val ) = each ( $HTTP_POST_VARS ) ) {
		$postVar = "post" . $key;
		$$postVar = $val; 
		}

	require( "../site.inc" );
	include( $_SITE["mirrordir"] . "sessions.inc" );

	/* user authentication */
	include( $_SITE["mirrordir"] . "userauth.inc" );

	if( ! isset( $op ) || $op == "" ) {
		$op = "login";
		} /* no operation selected */

	/* handle redirection */
	switch( $op ) {
		case "return":
		case "login":
			if( isAuthenticated() ) {
				/* redirect to $redirect */
				if( isset($redirect) && $redirect != "" ) {
					/* PHP only saves session data at end
					   of output, not end of script execution. */
					sessionSave();
					header( "Location: $redirect" );
					} else {
					/* PHP only saves session data at end
					   of output, not end of script execution. */
					sessionSave();
					header( "Location: " . $_SITE["docrootdir"] . "index.php" );
					}
				}
		break;
		
		case "logout":
			logout();
			/* redirect to $redirect; no need to save session
			   data, now that we've ended the session. */
			header( "Location: $redirect" );
		break;
		} /* end redirect operations switch */
?>
<?php
	/* HEADER */
	require( $_SITE["portaldir"] . "CommonHeader.ihtml" );
	require( $_SITE["portaldir"] . "StaticHeader.ihtml" );
?>
<?php
	switch( $op ) {
		case "login":
			include( $_SITE["mirrordir"] . "login.ihtml" );
		break;

		case "email":
			include( $_SITE["mirrordir"] . "email.ihtml" );
		break;

		case "change":
			include( $_SITE["mirrordir"] . "change.ihtml" );
		break;

		case "newacct":
			include( $_SITE["mirrordir"] . "newacct.ihtml" );
		break;

		case "edit":
			include( $_SITE["mirrordir"] . "edit.ihtml" );
		break;
		} /* end non-redirect operations switch */
?>

<?php
	if( ! ( $useUserName == false &&
		$usePassword == false && 
		$useVerified == false &&
		$useEmail == false ) ) {
		print( '<FORM METHOD="POST" ACTION="user.php' );
		if( isset( $redirect ) && $redirect != "" ) {
			print( "?redirect=" . urlencode($redirect) );
			}
		print( '">' );
		} /* end if I'll be doing anything with the form. */
?>

<TABLE BORDER=0 WIDTH="100%" CELLSPACING=4 CELLPADDING=0>
<?php
	if( $useUserName ) {
		print( '<TR><TD ALIGN="RIGHT">UserName:</TD>' . "\n" );
		print( '<TD><INPUT TYPE="TEXT" MAXLENGTH=64 SIZE=32 ' . "\n" );
		print( 'NAME="username" VALUE="' );
		if( isset( $postusername ) ) { print( $postusername ); }
		else if( isset( $username ) ) { print ( $username ); } 
		print( '"></TD></TR>' . "\n\n" );
		}
	if( $usePassword ) {
		print( '<TR><TD ALIGN="RIGHT">password:</TD><TD>' . "\n" );
		print( '<INPUT TYPE="PASSWORD" MAXLENGTH=64 SIZE=32 ' . "\n" );
		print( 'NAME="password" VALUE="' );
		if( isset( $postpassword ) ) { print ( $postpassword ); }
		else if( isset( $password ) ) { print ( $password ); }
		print( '"></TD></TR>' . "\n\n" );
		}
	if( $useVerified ) {
		print( '<TR><TD ALIGN="RIGHT">verify password:</TD><TD>' . "\n" );
		print( '<INPUT TYPE="PASSWORD" MAXLENGTH=64 SIZE=32 ' . "\n" );
		print( 'NAME="verified" VALUE="' );
		if( isset( $postverified ) ) { print ( $postverified ); }
		else if( isset( $verified ) ) { print ( $verified ); }
		print( '"></TD></TR>' . "\n\n" );
		}
	if( $useEmail ) {
		print( '<TR><TD ALIGN="RIGHT">e-mail:</TD><TD>' . "\n" );
		print( '<INPUT TYPE="TEXT" MAXLENGTH=64 SIZE=32 ' . "\n" );
		print( 'NAME="email" VALUE="' );
		if ( isset( $postemail ) ) { print ( $postemail ); }
		else if( isset( $email ) ) { print ( $email ); }
		print('"></TD></TR>' . "\n\n" );
		}

	print( '<TR><TD></TD><TD ALIGN=CENTER><INPUT TYPE="SUBMIT" NAME="op" ' );
	switch( $op ) {
		case "edit":
		case "login":
			print( 'VALUE="login">' );
		break;

		case "change":
			print( 'VALUE="change">' );
		break;

		case "email":
			print( 'VALUE="email">' );
		break;

		case "newacct":
			print( 'VALUE="newacct">' );
		break;
		} /* end button switch */
	print( '</TD></TR>' );
?>
</TABLE>
</FORM>

<?php
	/* FOOTER */
	require( $_SITE["portaldir"] . "StaticFooter.ihtml" );
	require( $_SITE["portaldir"] . "CommonFooter.ihtml" );
?>
