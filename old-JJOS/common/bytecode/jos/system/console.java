/**
 * console.java
 *
 * a single virtual console.
 */

package jos.system;

import jos.platform.*;
import jos.platform.event.*;
import jos.platform.driver.*;

public class console extends Object implements KeyListener, ConsoleDriver {
	ConsoleDriver dc = null;

	public console () {
		/* by default, a virtual console */
		dc = new jos.platform.driver.console();
		}

	/* construct this console with some given driver */
	public console ( ConsoleDriver cd ) {
		dc = cd;
		}

	public void keyPressed( KeyEvent ke ) { 
		this.putChar( ke.getChar() );
		} /* end keyPressed() */

	public void keyReleased( KeyEvent ke ) {
		this.putChar( ke.getChar() );
		} /* end keyReleased() */

	public void keyTyped( KeyEvent ke ) {
		/* this where the UNIX pipes to the shell
		   would take place.  Individual apps
		   could register against this console to recieve
		   the "raw mode" input from keyPressed/keyReleased
		   above. */
		this.putChar( ke.getChar() );
		} /* end keyTyped() */

	/* It's easiest for the driver to track whether or not
	   it's displayed. */
	public void focusGained ( FocusEvent fe ) {
		dc.focusGained(fe);
		} /* end focusGained() */

	public void focusLost ( FocusEvent fe ) {
		dc.focusLost(fe);
		} /* end focuseLost() */

	/* the console interface -- which should have an interface
	   classfile somewhere, probably. (+focusListener) */
	public void putChar(char c) { dc.putChar(c); }
	public char getChar(int row, int col) { return dc.getChar(row,col); }
	public void setChar(int x, int y, char c) { dc.setChar(x,y,c); }
} /* end class console */
