/**
 * KeyboardInterpreter.java
 *
 * KeyboardInterpreter is the abstract superclass
 * extended by driver.KeyboardInterpreter.
 */

package jos.platform;

import jos.platform.driver.*;
import jos.platform.event.*;
import java.util.*;

public abstract class KeyboardInterpreter implements ScancodeListener {
      /* Things that the driver needs to do */     
      public abstract void handleScancode( int s );
      
      /* When we need to translate JOS to AWT KeyEvents.
       * Also handy in general. */
      public abstract boolean isShiftDown( KeyEvent ke );
      public abstract boolean isControlDown( KeyEvent ke );
      public abstract boolean isAltDown( KeyEvent ke );
      public abstract boolean isMetaDown( KeyEvent ke );

      /* the KeyListeners will be handled for the driver here.*/
      protected Vector listeners = new Vector();

      public void addKeyListener( KeyListener kl ) {
            listeners.addElement( kl );
            } /* end addKeyListener */

      public void removeKeyListener( KeyListener kl ) {
            listeners.removeElement( kl );
            } /* end removeKeyListener */
      
      protected void dispatchKeyEvent( KeyEvent ke ) {
//	    System.err.println( "KI.dispatchKeyEvent(): dispatching event..." );
            Enumeration e = listeners.elements();
            KeyListener kl;

            while( e.hasMoreElements() ) {
                  kl = (KeyListener)e.nextElement();
                  switch ( ke.getType() ) {
                        case KeyEvent.KEY_PRESSED:
//			      System.err.println( "KI.dispatchKeyEvent(): KEY_PRESSED" );
                              kl.keyPressed( ke );
                              break;
                        case KeyEvent.KEY_RELEASED:
//	                      System.err.println( "KI.dispatchKeyEvent(): KEY_RELEASED" );
                              kl.keyReleased( ke );
                              break;
                        case KeyEvent.KEY_TYPED:
//	          	      System.err.println( "KI.dispatchKeyEvent(): KEY_TYPED" );
                              kl.keyTyped( ke );
                              break;
                        } /* end switch */
                  } /* end while */
            } /* end dispatchKeyEvent() */

      } /* end class KeyboardInterpreter */
