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

/**
 * Queue.java
 */

package jos.util;
import jos.util.QueueElement;

public class Queue extends Object {
	QueueElement first = null;
	QueueElement last = null;

	public Queue() {
		;
		} /* end constructor */

	public synchronized void push( Object o ) {
//		System.out.print( "Q: adding " );
//		System.out.println( o );
		QueueElement n = new QueueElement( o );
		if ( last != null ) {
			last.next = n;
			}
		last = n;
		if ( first == null ) {
			first = last;
			}
//		System.out.print( "Q: is now: " );
//		printQueue();
//		System.out.println( "." );
		notifyAll();
		} /* end push() */

	public synchronized void printQueue() {
		QueueElement n = first;
		
		System.out.print( n.value );
		System.out.print( " " );

		while ( n.next != null ) {
			n = n.next;
			System.out.print( n.value );
			System.out.print( " " );
			} /* end printing loop */
		} /* end printQueue */	

	public synchronized Object pop() {
		if ( first == null ) { /* Q is empty */
			while ( true ) {
			 try { wait(); break; }
			 catch( InterruptedException ie ) {
			   System.err.println( "jos.util.Queue interrupted waiting to be filled!" );
			  } /* end catch */
			 } /* end waiting loop */
			} /* wait for push to fill Q */

		QueueElement q = first;
		first = q.next;
//		System.out.print( "Q: popped, is now: " );
//		printQueue();
//		System.out.println( "." );
		return q.value;
		} /* end pop() */
		
	} /* end class Queue */
