/**
 * keyboardd.java
 *
 * keyboardd.java dispatches events
 * from a ByteQueue to a Vector of interested
 * ScancodeListeners
 */

package jos.platform.driver;

import jos.platform.*;
import jos.platform.event.*;
import jos.system.*;
import jos.util.*;
import java.util.*;

public class keyboardd {

     protected ByteQueue bytes;
     protected Vector listeners;

     public keyboardd( ByteQueue b, Vector l ) {
//      System.err.println( "keyboardd(): initializing byte queue..." );
      bytes = b;
//      System.err.println( "keyboardd(): initializing listener list..." );
      listeners = l;
      }

     /* handled by jJOS; try with FAST_NATIVE enabled */
     public native void start();

     public void run() {
//      System.err.println( "keyboardd.run(): daemon ready." );
      byte b;
      Enumeration e;
      
      while ( bytes != null && listeners != null ) {
            b = bytes.pop();
//	    System.err.println( "keyboardd.run(): popped scancodes from queue..." );
            synchronized( listeners ) { e = listeners.elements(); }
            ScancodeListener sl;

            while ( e.hasMoreElements() ) {
                  sl = (ScancodeListener)e.nextElement();
//		  System.err.println( "keyboardd.run(): calling handleScancode()" );
                  sl.handleScancode( b );
                  } /* end dispatch loop */
//	    System.err.println( "keyboardd.run(): completed dispatch." );
            } /* end run loop */
//    System.err.println( "keyboardd.run(): daemon exiting..." );
      } /* end run() */

} /* end keyboardd */
 
