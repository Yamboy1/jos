/**
 * driver.keyboard
 *
 * driver.keyboard is a hardware driver
 * that implements the "ScanscodeSpeaker"
 * interface.
 */

package jos.platform.driver;

import jos.platform.*;
import jos.platform.event.*;
import jos.system.*;
import jos.util.*;
import java.util.*;

public class keyboard extends Object implements InterruptListener {

   public keyboard () {
      /* Attatch to the keyboard interrupt */
//      System.err.println( "driver.keyboard(): initializing interrupts..." );
      interrupts.addInterruptListener( 9, this );
//      System.err.println( "driver.keyboard(): initializing keyboard..." );
      handleInterrupt(9); /* probably not the Right Thing, but it seems to work. */

      /* spawn & start a dispatch thread */
      /* $: synchronization issues with listeners? */
//      System.err.println( "driver.keyboard(): initializing keyboard daemon..." );
      keyboardd kd = new keyboardd( bytes, listeners );
//      System.err.println( "driver.keyboard(): starting keyboard daemon..." );
      kd.start();
      }

   public void handleInterrupt( int n ) {
      /* if it's not a keyboard interrupt somehow,
       * ignore it. */
      if ( n != 9 ) { return; }

//      System.err.println( "driver.keyboard(): handling keyboard interrupt." );

        /* fetch the scancode(s) that generated
         * this interrupt, and enqueue them. */
        byte status;
        byte scancode;

	/* ignores the possibility of a mouse (PS/2) */
	status = jos.system.machine.in8( (short)0x64 );
      
	do {
		scancode = jos.system.machine.in8( (short)0x60 );
		if ( (status & (byte)0x01) != 0 ) {
//			System.err.println( "pushing scancode..." );
			bytes.push( scancode );
//			System.err.println( "pushed scancode..." );
			}
		status = jos.system.machine.in8( (short)0x64 );
	} while ( (status & (byte)0x01) != 0 );
	
	/* re-enable keyboard */
	while( (jos.system.machine.in8((short)0x64) & (byte)0x02) != 0 );
	jos.system.machine.out8( (short)0x64, (byte)0xAE );	    
      } /* end handleInterrupt */

   protected Vector listeners = new Vector();
   protected ByteQueue bytes = new ByteQueue();

   public void addScancodeListener( ScancodeListener sl ) {
      /* synchronized( listeners ) */ { listeners.addElement( sl ); }
      } /* end addScancodeListener */

   public void removeScancodeListener( ScancodeListener sl ) {
      /* synchronized( listeners ) */ { listeners.removeElement( sl ); }
      } /* end removeScancodeListener */      

} /* end driver.keyboard */
