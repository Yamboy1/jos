/**
 * console.java
 *
 * a single virtual console,
 * implemented in (PC) VGA text mode.
 */

package jos.platform.driver;

import jos.system.*;
import jos.platform.event.*;

public class consoleNative extends Object implements ConsoleDriver {

	protected native void setVisible( boolean v );
	public native void putChar( char c );
	public native void setChar( int row, int column, char c );
	public native char getChar( int row, int column );

	public void focusGained( FocusEvent fe ) {
		setVisible( true );
		}

	public void focusLost( FocusEvent fe ) {
		setVisible( false );
		}
} /* end class consoleNative */

