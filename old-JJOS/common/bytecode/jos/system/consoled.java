/*
 * consoled.java
 *
 * manages virtual consoles
 */

package jos.system;

import jos.platform.*;
import jos.platform.event.*;

/* -_Quinn: this should be a soliton class. */
public class consoled extends Object implements KeyListener {

	/* the virtual consoles */
	protected console[] consoles = new console[12];
	protected int current_console = 0;

	public consoled () {
		;			
		} /* end consoled() */

	public void init() {
//		System.err.println( "consoled.init(): generating consoles..." );
		consoles[0] = new console (new jos.platform.driver.consoleNative());
		for (int x = 1; x < 12; x++ ) {
			consoles[x] = new console();
			}
		/* #7 is traditionally VGA... */
		consoles[6] = new console( new jos.platform.driver.consoleVGA() );

//		System.err.println( "consoled.init(): setting root console..." );
		consoles[0].focusGained( new FocusEvent() );

		/* set up the event tree */
//		System.err.println( "consoled.init(): generating event tree..." );
		keyboard.getKeyboard().addKeyListener( this );

//		System.err.println( "consoled.init(): initialization complete..." );
		} /* end init() */

	public void keyTyped( KeyEvent ke ) {
		/* pass the event through to the current console */
	    	consoles[current_console].keyTyped( ke );
            	} /* end keyTyped() */

	public void keyPressed( KeyEvent ke ) {
		if ( ke.getVKcode() >= KeyEvent.VK_F1 && ke.getVKcode() <= KeyEvent.VK_F12 ) {
			int which = ke.getVKcode() - KeyEvent.VK_F1;
			if ( which >= 12 ) { /* something terrible has happened */
				System.err.print( "system.consoled(): attempt to switch to invalid virtual console #" );
				System.err.println( which );
				return;
				}
			consoles[current_console].focusLost( new FocusEvent() );
			current_console = which;
			consoles[current_console].focusGained( new FocusEvent() );
			return;
			}

	 	/* pass the event through to the current console */
	    	consoles[current_console].keyPressed( ke );	    
            	} /* end keyPressed() */

	public void keyReleased( KeyEvent ke ) {
   		/* pass the event through to the current console */
	    	consoles[current_console].keyReleased( ke );
	    	} /* end keyReleased() */

} /* end class consoled */
