/**
 * keyboard.java
 * 
 * keyboard.java virtualizes the keyboard
 * to a series of KeyEvents, KEY_PRESSED,
 * KEY_RELEASED, and KEY_TYPED.
 * 
 * The lower driver, jos.platform.driver.keyboard,
 * has its lower edge at the H/W (via jos.system.machine)
 * and its higher edge here, at jos.platform.keyboard.
 * jpdK feeds keyboard.java KEY_PRESSED and KEY_RELEASED type
 * events with only the scancode value present.  ("raw" events)
 * 
 * The higher driver, jos.platform.driver.KeyboardInterperter
 * has its lower edge at ScancodePressed(), recieving the
 * "raw" events, and its upper edge generating higher-order
 * events which it then dispatches to its listeners
 * ( i.e. keyboard.java )
 */

package jos.platform;

import jos.platform.driver.*;
import jos.platform.event.*;
import java.util.*;

public class keyboard extends Object implements ScancodeListener, KeyListener {

     /* The particular KeyboardInterperter
      * currently in use.  This is the default
      * KeyboardInterperter; nothing prevents a
      * specfic app (the shell) from connecting
      * its own KeyboardInterperter to the raw-mode
      * output of jos.platform.keyboard, thereby
      * generating per-user keymaps.
      */
     protected KeyboardInterpreter ki;

     /* The particular keyboard driver
      * currently in use.
      */
     protected jos.platform.driver.keyboard kd;

     /* The listener vectors */
     Vector key_listeners = new Vector();
     Vector sc_listeners = new Vector();
    
     public void init () {
      /* generate the default drivers */
//      System.err.println( "keyboard.init(): generating keyboard interpreter..." );
      ki = new jos.platform.driver.KeyboardInterpreter();
//      System.err.println( "keyboard.init(): generating keyboard driver..." );
      kd = new jos.platform.driver.keyboard();

      /* set up the listener tree;
         platform.keyboard listens to its driver,
         which feeds scancode events to it (via keyboardd);
         platform.keyboard feeds these events to the ki,
         which fires the interpreted KeyEvents back at
         platform.keyboard for dispatch. */
//      System.err.println( "keyboard.init(): generating event tree..." );
      kd.addScancodeListener( this );
      this.addScancodeListener( ki );
      ki.addKeyListener( this );
//      System.err.println( "keyboard.init(): initialization complete..." );
      } /* end init() */

      /* dispatch incoming events */
      public void keyTyped( KeyEvent ke ) {
	    dispatchKeyEvent( ke );
            } /* end keyTyped() */
     
      public void keyPressed( KeyEvent ke ) {
	    dispatchKeyEvent( ke );
            } /* end keyPressed() */
            
      public void keyReleased( KeyEvent ke ) {
	    dispatchKeyEvent( ke );
            } /* end keyReleased() */

      public void handleScancode( int s ) {
            /* This will cause ki to fire back key*() events */
            ki.handleScancode( s );

            /* distribute to scancode listeners */
            ;
            } /* end handleScancode */
      
     public void addKeyListener( KeyListener kl ) {
      key_listeners.addElement( kl );
      } /* end addKeyListener */

     public void removeKeyListener( KeyListener kl ) {
      key_listeners.removeElement( kl );
      } /* end removeKeyListener */

     public void addScancodeListener( ScancodeListener sl ) {
      sc_listeners.addElement( sl );
      } /* end addScancodeListener */

     public void removeScancodeListener( ScancodeListener sl ) {
      sc_listeners.removeElement( sl );
      } /* end removeScancodeListener */

     protected static keyboard k;

     public static void setKeyboard( keyboard k ) {
	keyboard.k = k;
	}

     public static keyboard getKeyboard() {
	return keyboard.k;
	}

     public void dispatchKeyEvent ( KeyEvent ke ) {
	Enumeration e = key_listeners.elements();
	KeyListener kl;
	while ( e.hasMoreElements() ) {
		kl = (KeyListener)e.nextElement();
		
		switch( ke.getType() ) {
			case ke.KEY_PRESSED:
				kl.keyPressed( ke );
				break;
			case ke.KEY_RELEASED:
				kl.keyReleased( ke );
				break;
			case ke.KEY_TYPED:
				kl.keyTyped( ke );
				break;
			} /* end switch */
		} /* end while */
	} /* end dispatchKeyEvent */

} /* end jos.platform.keyboard */      
