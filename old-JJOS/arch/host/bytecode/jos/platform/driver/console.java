/**
 * console.java
 *
 * a single virtual console,
 * implemented in (PC) VGA text mode.
 */

package jos.platform.driver;

import jos.system.*;
import jos.platform.event.*;

public class console extends Object implements ConsoleDriver {
	public static final int JJSCREENWIDTH = 80;
	public static final int JJSCREENHEIGHT = 24;

	public void focusGained( FocusEvent fe ) {
		displayed = true;

		/* stolen from Thomas Bocek's VGA code;
		   should set the VGA card to text mode. */
/*
		jos.system.machine.out8( (short)0x3C4, (byte)0 );
		jos.system.machine.out8( (short)0x3C4, (byte)1 );
		jos.system.machine.out8( (short)0x3C2, (byte)((byte)((byte)jos.system.machine.in8((short)0x3CC)) & (byte)0xFE) );
		jos.system.machine.out8( (short)0x3C4, (byte)0 );
		jos.system.machine.out8( (short)0x3C4, (byte)3 );
*/

		update();
		}
	public void focusLost( FocusEvent fe ) {
		displayed = false;
		}

	public char getChar( int row, int column ) {
		return (char) buffer[ (row * 80 * 2) + (column * 2) ];
		}

	public void setChar( int row, int column, char c ) {
		buffer[ (row * JJSCREENWIDTH * 2) + (column * 2) ] = (byte)c;
		buffer[ (row * JJSCREENWIDTH * 2) + (column * 2) + 1 ] = 0x07;

		if (displayed) {
			vga[ (row * JJSCREENWIDTH * 2) + (column * 2) ] = (byte)c;
			vga[ (row * JJSCREENWIDTH * 2) + (column * 2) + 1 ] = 0x07;
			}
		}

	protected int row = 0;
	protected int column = 0;

	protected byte[] vga = jos.system.machine.mapByteArray( 0xB8000, JJSCREENWIDTH * JJSCREENHEIGHT * 2 );
	protected byte[] buffer = new byte[ JJSCREENWIDTH * JJSCREENHEIGHT * 2];

	protected boolean displayed = false;

protected void update () {
	System.arraycopy( buffer, 0, vga, 0, buffer.length );
	}

protected void scrollUpOneLine() {
	System.arraycopy( buffer, JJSCREENWIDTH * 2, buffer, 0, buffer.length - JJSCREENWIDTH * 2 );

	/* fill in last row with blanks */
	for ( int rcol = 0; rcol < JJSCREENWIDTH; rcol++ ) {
		setChar( JJSCREENHEIGHT - 1, rcol, ' ' );
		}

	/* if we're 'live', update the display */
	if ( displayed ) { update(); }
	} /* end scrollUpOneLine() */

public void putChar( char c ) {
	if (c == '\n') {
     		if (row < (JJSCREENHEIGHT-1)) {
	          column = 0;
        	  row++;
       		  } else {
	          scrollUpOneLine();
        	  column = 0;
        	  }
        	} else {
     		if (column < (JJSCREENWIDTH-1)) {
	          setChar(row, column, c );
        	  column++;
        	  } else {
	          putChar('\n');
        	  putChar(c);
        	  }
        	} /* end else */
	} /* end putCharacter() */

} /* end class console */

