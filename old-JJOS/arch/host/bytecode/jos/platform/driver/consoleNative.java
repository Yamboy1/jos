/**
 * consoleNative.java
 *
 * mapByteArray() doesn't do anything
 * useful on the host build, so fake
 * doing things here.
 */

package jos.platform.driver;

import jos.system.*;
import jos.platform.event.*;

public class consoleNative extends console implements ConsoleDriver {
	public void putChar( char c ) {
		System.err.print( c );
		} /* end putChar() */
	} /* end class console */
