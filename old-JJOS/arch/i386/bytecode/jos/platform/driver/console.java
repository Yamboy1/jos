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
		update();
		}

	private void putString (String s) {
		for (int i = 0; i < s.length(); i++) {
			putChar( s.charAt(i) );
			}
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
	updateCursor();
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

/* namespace conflicts? */
public void setCursor( int row, int col ) {
  /* the cursor is set by a 16-bit offset
     into the framebuffer */
  short cursor_addr = (short)((80 * 2 * row) + (col * 2));
  /* some random VGA breakage */
  cursor_addr /= 2;
  byte high = (byte)((cursor_addr >> 8) & 0xff);
  byte low = (byte)(cursor_addr & 0xff);

  /* output the high byte */
  jos.system.machine.out8( (short)0x3d4, (byte)0x00e );
  jos.system.machine.out8( (short)0x3d5, (byte)high );
  /* and the low byte */
  jos.system.machine.out8( (short)0x3d4, (byte)0x00f );
  jos.system.machine.out8( (short)0x3d5, (byte)low );

  /* provisionally, set the cursor's mode */
  buffer[ (row * JJSCREENWIDTH * 2) + (col * 2) + 1 ] = 0x07;
  if ( displayed ) {
	vga[ (row * JJSCREENWIDTH * 2) + (column * 2) + 1 ] = 0x07;
	}
  } /* end setCursor() */

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
		  updateCursor();
        	  } else {
	          putChar('\n');
        	  putChar(c);
        	  }
        	} /* end else */
	} /* end putCharacter() */

protected void updateCursor() {
          /* Put the cursor in the right place */
       	  if( column < (JJSCREENWIDTH-1) ) {
                /* we can put the cursor on the current row */
       	        setCursor( row, column );
      		} else {
               	/* we need to put the cursor a line further down */
               	if ( row + 1 < (JJSCREENHEIGHT-1) ) {
                       	/* we can put the cursor on the next row */
                       	setCursor( row + 1, 0 );
                       	} else {
                       	/* we need to scroll */
                       	putChar('\n');
                       	setCursor( row, 0 );
                       	} /* end if not on current line */
               	} /* end if on current line */
	} /* end updateCursor() */

} /* end class console */

