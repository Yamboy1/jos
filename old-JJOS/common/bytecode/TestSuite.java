/*
 *    The contents of this file are subject to the JJOS Public License
 *    Version 0.0 (the "License"); you may not use this file except in
 *    compliance with the License. You may obtain a copy of the License at
 *    http://www.jos.org/
 *
 *    Software distributed under the License is distributed on an "AS IS"
 *    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 *    License for the specific language governing rights and limitations
 *    under the License.
 *
 *    The Original Code is decaf's code, released October, 1998.
 *
 *    The Initial Developer of the Original Code is Todd L. Miller.
 *    Portions created by Todd L. Miller  are Copyright (C) 1998 & 1999.
 *    All Rights Reserved.
 *
 */

import java.io.*;
import java.util.Vector;
import java.util.Enumeration;

import jos.system.interrupts;
import jos.system.keyboard;
import jos.system.consoled;
import jos.platform.driver.StandardVga;

public class TestSuite {

    /* tests in order of execution */
    public static boolean testVM = true;
    public static boolean testThreads = true;
    public static boolean testExceptions = true;
    public static boolean testVGADriver = true;
    public static boolean testGC = true;
    public static boolean testConsoleDaemons = true;

    /* the jos logo -> just for testing */
    final static int[] logo =
    {0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0,
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
     0,0,0,0,0,14,15,15,15,15,14,0,0,0,0,0};

    public void runTests () {
	if (testVM)
	    testVM();
	else
	    System.out.println( "skipping VM tests..." );

	if (testThreads)
	    testThreads();
	else
	    System.out.println( "skipping threading tests..." );

	if (testExceptions)
	    testExceptions();
	else
	    System.out.println( "skipping exception tests..." );

	if (testVGADriver)
	    testVGADriver();
	else
	    System.out.println( "init() skipping VGA tests..." );
		
	if (testGC)
	    testGC();
	else
	    System.out.println( "init() skipping GC tests..." );

	if (testConsoleDaemons)
	    testConsoleDaemons();
	else
	    System.out.println( "init() skipping console daemon tests..." );

    }

    public static void testCasting() {
	System.out.println( "init() testing casting..." );

	Object o = new Object();
	Vector v = new Vector();
	int[] array_i = new int[10];
	String[] array_s = new String[10];
	
	System.out.println( "o = (Object)Vector; (OK)" );
	o = (Object)v;

	System.out.println( "Vector = (Vector)o; (OK)" );
	v = (Vector)o;

	System.out.println( "o = Object; Vector = (Vector)o (NOT OK)" );
	o = new Object();
	v = (Vector)o;

	System.out.println( "ser = new String(); (OK)" );
	System.out.println( "o = (Object)int[]; (OK)" );
	o = (Object)array_i;

	System.out.println( "o = (Object)String[]; (OK)" );
	o = (Object)array_s;

	System.out.println( "o = (Object)int[]; int[] = (int[])o; (OK)" );
	o = (Object)array_i;
	array_i = (int[])o;

	// Failed by compiler.
	// System.out.println( "s = (String[])int[]; (NOT OK)" );
	// array_s = (String[])array_i;

	// Failed by compiler.
	// System.out.println( "i = (int[])Vector; (NOT OK)" );
	// array_i = (int[])v;

	o = new Object();
	v = new Vector();
	Enumeration e = v.elements();
	System.out.println( "e = (Enumeration)v; (NOT OK)" );
	e = (Enumeration)v;

	e = v.elements();
	System.out.println( "v = (Vector)e; (NOT OK)" );
	v = (Vector)e;

	System.out.println( "ser = new String(); (OK)" );
	Serializable ser = new String();
    } /* end testCasting() */
		

    public static boolean testFunctionCalls() {
	// return true on error.
	if ( testStaticDoubleReturn() != (double)5.77 ) {
	    System.out.println( "Static double return failed." );
	    return true;
	}	
	if ( testDynamicDoubleReturn((double)5.77) != (double)5.77 ) {
	    System.out.println( "Dynamic double return failed." );
	    return true;
	}

	// float, int, reference(array), void(null) remain to test.

	return false;	
    } // end testFunctionCalls()

    public static double testStaticDoubleReturn() { return (double)5.77; }
    public static double testDynamicDoubleReturn( double d ) { return d; }

    public static void testMathOps() {
	System.out.print( "testing integer comparisons... " );
       
	int k = 1;
	int l = 10;
	System.out.print( "ops failed: " );
	if ( k == l ) { System.out.print( "== " ); }
        if ( k != l ) { ; } else { System.out.print( "!= " ); }
        if ( k > l ) { System.out.print( "> " ); }
        if ( l < k ) { System.out.print( "< " ); }
        if ( k >= l ) { System.out.print( ">= " ); }
        if ( l <= k ) { System.out.print( "<=" ); }
	System.out.print( "\n" );

	System.out.print( "testing double comparisons... " );

        double o = (double)55.3;
        double p = (double)11.1;
	System.out.print( "ops failed: " );
        if ( o == p ) { System.out.print( "== " ); }
        if ( o != p ) { ; } else { System.out.print( "!= "); }
        if ( o < p ) { System.out.print( "< " ); }
        if ( p > o ) { System.out.print( "> " ); }
        if ( o <= p ) { System.out.print( "<= " ); }
        if ( p >= o ) { System.out.print( ">=" ); }
	System.out.print( "\n" );

        System.out.print( "Testing float comparisons... " );

        float q = (float)4.5;
        float r = (float)4.5;
	System.out.print( "ops failed: " );
        if ( q == r ) { ; } else { System.out.print( "== "); }
        if ( q != r ) { System.out.print( "!= " ); }
        if ( q < r ) { System.out.print( "< " ); }
        if ( r > q ) { System.out.print( "> " ); }
        if ( r <= q ) { ; } else { System.out.print( "<= " ); }
        if ( q >= r ) { ; } else { System.out.print( ">=" ); }
	System.out.print( "\n" );

        System.out.print( "Testing float ops... " );

	System.out.print( "ops failed: " );
        if ( q + r != (float)9.0 ) { System.out.print( "add " ); }
        if ( q - r != (float)0.0 ) { System.out.print( "sub " ); }
        if ( q / r != (float)1.0 ) { System.out.print( "div " ); }
        if ( q % r != (float)0.0 ) { System.out.print( "mod " ); }
        if (    -q != (float)-4.5 ) { System.out.print( "neg " ); }
        if ( q * r != (float)20.25 ) { System.out.print( "mul" ); }
        System.out.print( "\n" );

        System.out.print( "Testing double ops... " );

	double result;
	System.out.print( "ops failed: " );
	result = o + p;
        if ( result < 66.0 || result > 67.0 ) { System.out.print("add "); }
	result = o - p;
        if ( result < 44.0 || result > 45.0 ) { System.out.print("sub "); }
	result = -o;
        if ( result > -55 || result < -56 ) { System.out.print("neg "); }
	result = o * p;
        if ( result < 613 || result > 614 ) { System.out.print( "mul" ); }
        System.out.print( "\n" );

	// need long ops & comparisons, integer ops;
  	// byte & char ops * comparisons.
    } // end testMathOps()

    public static void testArrayOps() {
	System.out.print( "testing array operations... " );	

	int[][][] array_three = new int [5][5][5];
	array_three[4][4][4] = 3;

	int[][] array_two = new int [4][4];
	array_two[3][3] = 2;

	int[] array_one = new int [3];
	array_one[2] = 1;

	int two = array_two[3][3];
	int three = array_three[4][4][4];
	int one = array_one[2];

	if( one != 1 ) { System.out.print( "1-dim int array failed. " ); }
	if( two != 2 ) { System.out.print( "2-dim int array failed. " ); }
	if( three != 3 ) { System.out.print( "3-dim int array failed." ); }
	System.out.print( "\n" );

	String[] strings = new String[3];
	strings[1] = "one";
	System.out.print( "\"one\" should be printed: " );
	System.out.println( strings[1] );
    } // end testArrayOps()

    public static void testVM() {
	System.out.println( "testing the VM..." );
			
	if ( testFunctionCalls() ) {
	    System.out.println( "testFunctionCalls() failed!" );
	    System.out.println( "the remaining tests are probably invalid." );
	}	
	testMathOps();
	testArrayOps();
	testCasting();

	/* test print/println */
	System.out.print( "does 56 = " );
	System.out.print( 56 );
	System.out.println( "?" );

	System.out.print( "is true = " );
	System.out.print( true );
	System.out.println( "?" ) ;

	System.out.print( "This sentence should end with a period" );
	System.out.println( (char)'.' );	
    }

    public static void testThreads() {
	System.out.println( "init(): testing threading." );
	System.out.println( "init() starting counter..." );
	counter c = new counter();
	c.start();

	System.out.println( "init() starting decounter..." );
	counter d = new counter( false );
	d.start();

	System.out.println( "init() entering .wait() loop..." );
	int interrupt_count = 0;
	while ( interrupt_count < 10 ) {
				/* Bad Karma w/o this. */
	    synchronized( jos.system.interrupts.int000 ) {
		/* because wait() calls release
		   locks, this doesn't actually
		   cause any blockage. */
		try { jos.system.interrupts.int000.wait(); }
		catch ( InterruptedException ie ) {
		    System.out.println( "init() interrupted waiting for notify" );
		    continue;
		}
	    } /* end synchlock on interrupt. */
				// System.out.println( "init() caught interrupt." );
	    interrupt_count++;
	} /* end .wait() loop */
	System.out.println( "init() has caught 10 interrupts." );
    }

    public static void testExceptions() {
	/* run exception tests */
	System.out.println( "init() testing exceptions()" );

	/* throw/catch pair #1 -- basic mechanism */
	try {
	    throw new Exception();
	} /* end throw #1 */
	catch ( Exception e ) {
	    System.out.println( "init(): caught exception #1!" );
	} /* end catch #1 */

	/* throw/catch pair #2 -- casting exceptions up */
	try { throw new java.io.IOException(); }
	catch ( Exception e ) {
	    System.out.println( "init(): caught exception #2!" );
	} /* end catch #2 */

	/* throw/catch pair #3 -- data preservation */
	try { throw new Exception( "init(): caught exception #3!" ); }
	catch ( Exception e ) {
	    System.out.println( e.getMessage() );
	}
    }

    public static void testConsoleDaemons() {			
	System.out.println( "init() starting console daemons..." );

	System.out.println( "init() starting jos.system.keyboard..." );
	/* sets up the hardware driver */
	keyboard k = new keyboard();
	/* starts the KeyEvent dispatch loop */
	k.start();

	System.out.println( "init() starting jos.system.consoled..." );
	/* listens to the dispatching from j.s.k above */
	consoled cd = new consoled();
    }

    public static void testVGADriver() {
	System.out.println( "init(): testing VGA driver..." );
	StandardVga v = new StandardVga();
	if(v.setVideoMode(640,480,4)==0)
	    {
		v.startService();
		v.bitblt(312,232,logo,16);

		for (int y=0; y<480; y++)
		    for (int x=0; x<640; x++)
			v.drawPixel(x, y, 1);

		v.delay(0x100000);
		v.stopService();
		System.out.println("init():... done testing VGA driver.");
	    }
    }

    public static void testGC() {
	System.out.println( "testing GC ..." );

	int i=0;

	while (i<10000000)
	    {
		int size = i%1000;
		int[] garbage = new int[size];
		i = i+1;

		//		testVM();
		//		testThreads();
		//		testExceptions();
		//		testCasting();
	    }

	System.out.println( "testing GC ..." );
    }
}
