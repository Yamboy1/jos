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
	/* test procedure */
	protected void gradientFill() {
		for ( int x = 0; x < 640; x++ ) {
			for ( int y = 0; y < 480; y++ ) {
				buffer[ (x * 640) + y ] = (byte)(y + x );
				} /* end Y loop */
			} /* end X loop */
		} /* end gradientFill() */

	public void focusGained( FocusEvent fe ) {
		displayed = true;
		gradientFill();
		update();
		}

	public void focusLost( FocusEvent fe ) {
		displayed = false;
		System.arraycopy( vga, 0, buffer, 0, vga.length );		
		}

	public char getChar( int row, int column ) {
		return '!';
		}

	public void setChar( int row, int column, char c ) {
		return;
		}

	/* The VGA framebuffer address. */
	protected byte[] vga = jos.system.machine.mapByteArray( 0xA0000, 640 * 480 );
	protected byte[] buffer = new byte[ 640 * 480 ];

	protected boolean displayed = false;

	protected void update () {
		System.arraycopy( buffer, 0, vga, 0, buffer.length );
		}

	public void putChar( char c ) {
		return;
		} /* end putCharacter() */

} /* end class console */

