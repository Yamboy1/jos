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

/*
 * QueueElement
 */
 
package jos.util;

public class QueueElement {
	public Object value = null;
	public QueueElement next = null;

	public QueueElement ( Object o ) {
		value = o;
		}
	} /* end class QueueElement */
