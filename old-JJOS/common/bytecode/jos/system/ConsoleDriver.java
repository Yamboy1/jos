/**
 * ConsoleDriver.java
 *
 * interface to a single virtual console.
 */

package jos.system;

import jos.platform.*;
import jos.platform.event.*;
import jos.platform.driver.*;

public interface ConsoleDriver extends FocusListener {
	/* stream-oriented */
	public void putChar( char c );

	/* screen-oriented */
	public char getChar( int row, int col );
	public void setChar( int x, int y, char c );
/*
	public byte getAttribute( int row, int col );
	public void setAttribute( int row, int col );
*/
	
	/* public Point getCursor(); */
/*
	public void setCursor( int row, int col );
*/

	/* attribute constants -- VGA mode 7 */
	public static final short NORMAL = 0x07;
	public static final short INTENSE = 0x0f;
	public static final short UNDERLINE = 0x01;
	public static final short INTENSE_UNDERLINE = 0x09;
	public static final short REVERSED = 0x70;
	public static final short BLINKING_REVERSED = 0xf0;	
	
} /* end class ConsoleDriver */
