/**
 * init.java
 */

import jos.system.*;
import jos.platform.*;
import jos.platform.event.*;
import jos.platform.driver.standardVGA;

public class init {
	public static void main( String argv[] ) {
		System.out.println( "init for jJOS/decaf, version 0.2" );

		/* start the keyboard driver */
		System.err.println( "init: generating keyboard driver..." );
		keyboard.setKeyboard( new keyboard() );
		System.err.println( "init: initializing keyboard driver..." );
		keyboard.getKeyboard().init();

		/* start the console daemon */
		System.err.println( "init: generating console daemon..." );
		consoled cd = new consoled();
		System.err.println( "init: initializing console daemon..." );
		cd.init();

		/* test the VGA driver */
/*
		standardVGA vga = new standardVGA();
		vga.startService();
		vga.bitblt(312, 232, logo, 16 );
		vga.delay( 0x100000 ); // give us time to look at it
		vga.stopService();
*/

/*
		System.err.println( 100 );
		System.err.println( String.valueOf( 100 ) );
		System.err.println( cd.toString() );
*/

		/* If init were written as a thread, it would sleep()
	           here, but sleeping/blocking threads aren't counted as
		   live in the scheduler, which might not be the Right
		   Way to count. */
		while ( true ) { ; }
		} /* end main() */

        /* the jos logo */
        final static int[] logo = {
	0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0,
        0,0,0,14,15,7,3,7,3,7,3,15,14,0,0,0,
        0,0,14,15,7,3,7,3,7,3,7,3,15,14,0,0,
        0,14,15,6,6,6,3,7,3,7,3,7,3,15,14,0,
        0,15,6,1,1,1,6,3,7,3,7,3,7,3,15,0,
        14,6,1,1,1,1,1,6,3,7,3,7,3,7,3,14,
        15,1,1,1,1,1,1,6,7,3,7,3,7,3,7,15,
        15,1,1,3,7,1,1,6,3,7,3,1,1,7,3,15,
        15,1,1,7,3,1,1,1,6,3,7,1,1,3,7,15,
        15,1,1,1,1,1,1,1,6,7,3,7,3,7,3,15,
        14,1,1,1,1,1,1,1,6,3,7,3,7,3,6,14,
        0,15,1,1,1,1,1,1,1,6,3,7,3,6,15,0,
        0,14,1,1,1,1,1,1,1,1,6,6,6,1,14,0,
        0,0,14,1,1,1,1,1,1,1,1,1,1,14,0,0,
        0,0,0,14,15,1,1,1,1,1,1,15,14,0,0,0,
        0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0
	}; /* end JOS logo */

} /* end init */
