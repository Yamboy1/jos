/**
 * consoleVGA.java
 *
 * a single virtual console,
 * implemented as (PC) VGA, 640x480x16.
 */

package jos.platform.driver;

import jos.system.*;
import jos.platform.event.*;

public class consoleVGA extends Object implements ConsoleDriver {
	/* the VGA driver object-mode */
	standardVGA myVGA = new standardVGA();

	public void focusGained( FocusEvent fe ) {
		displayed = true;
		myVGA.startService();
		myVGA.bitblt( 0, 0, logo, 16 );
		/* see below -- put update() in here? */
		}

	public void focusLost( FocusEvent fe ) {
		displayed = false;
		/* should store the VGA framebuffer (to buffer)...
		   ask Bocek how to do this / if it should
		   be in the standardVGA driver */
		myVGA.stopService();
		}

	public char getChar( int row, int column ) {
		return '!';
		}

	public void setChar( int row, int column, char c ) {
		return;
		}

	public void putChar( char c ) {
		return;
		} /* end putCharacter() */

	/* see above */
	protected int[] buffer = new int[ 640 * 480 ];
	protected boolean displayed = false;

        /* the jos logo */
        final static int[] logo = {
        0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0,
        0,0,0,14,15,7,3,7,3,7,3,15,14,0,0,0,
        0,0,14,15,7,3,7,3,7,3,7,3,15,14,0,0,
        0,14,15,6,6,6,3,7,3,7,3,7,3,15,14,0,
        0,15,6,1,1,1,6,3,7,3,7,3,7,3,15,0,
        14,6,1,1,1,1,1,6,3,7,3,7,3,7,3,14,
        15,1,1,1,1,1,1,6,7,3,7,3,7,3,7,15,
        15,1,1,3,7,1,1,6,3,7,3,1,1,7,3,15,
        15,1,1,7,3,1,1,1,6,3,7,1,1,3,7,15,
        15,1,1,1,1,1,1,1,6,7,3,7,3,7,3,15,
        14,1,1,1,1,1,1,1,6,3,7,3,7,3,6,14,
        0,15,1,1,1,1,1,1,1,6,3,7,3,6,15,0,
        0,14,1,1,1,1,1,1,1,1,6,6,6,1,14,0,
        0,0,14,1,1,1,1,1,1,1,1,1,1,14,0,0,
        0,0,0,14,15,1,1,1,1,1,1,15,14,0,0,0,
        0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0
        }; /* end JOS logo */

} /* end class console */

