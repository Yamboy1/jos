/**
 * interrupts.java
 * 
 * The kernel disables interrupts
 * for the duration of the interrupt dispatch,
 * which generates based on knowledge of the Set class,
 * so drivers (a) must be threaded and (b) should
 * spend an absolute h/w-determined minimum of time
 * in its handleInterrupt() method.
 */

package jos.system;

import jos.platform.event.*;
import jos.util.Set;

public class interrupts extends Object {
     protected static Set[] handlers = new Set[16];

     public static void addInterruptListener( int n, InterruptListener il ) {
      Set v = handlers[n];
      if ( v == null ) { handlers[n] = new Set(); v = handlers[n]; }

      v.addElement( il );
      } /* end addInterruptListener */

     public static void removeInterruptListener( int n, InterruptListener il ) {
      Set v = handlers[n];
      if ( v == null ) { return; } /* $: Right Thing To Do? */

      v.removeElement( il );
      } /* end removeInterruptListener */
} /* end interrupts */     
