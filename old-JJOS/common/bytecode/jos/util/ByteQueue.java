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
 * ByteQueue.java
 *
 * a simple fixed-length Queue.
 */

package jos.util;

public class ByteQueue extends Object {
	final static int Q_SIZE = 10;

	protected byte [] data;
	protected int top = 0;
	protected int bottom = 0;

	public ByteQueue() {
		data = new byte[Q_SIZE+1];
		} /* end constructor */

	public synchronized void push( byte o ) {
		data[bottom] = o;
		bottom++;
		if ( bottom >= Q_SIZE ) {
			bottom = 0;
			} /* end overflow */
		/* if wrapped around, advance top so it
		   points to the old value */
		if ( top == bottom ) { top++; }
		notifyAll();
		} /* end push() */

	public synchronized byte pop() {
                if ( top == bottom ) { /* Q is empty */
                        while ( true ) {
                         try { wait(); break; }
                         catch( InterruptedException ie ) { ; }
                         } /* end waiting loop */
                        } /* wait for push to fill Q */
		byte r = data[top];

		top++;
		if ( top >= Q_SIZE ) {
			top = 0;
			} /* end overflow */

		return r;
		} /* end pop() */

	} /* end class ByteQueue */
