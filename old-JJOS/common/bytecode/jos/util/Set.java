/*
   Set.java

   used for communication with the kernel.
*/

package jos.util;

public class Set {
	private Object[] elements = new Object[SET_SIZE];
	private final static int SET_SIZE = 32;

	public void addElement( Object o ) {
		int x = 0;
		for( x = 0; x < SET_SIZE; x++ ) {
			if ( elements[x] == o ) { continue; }
			if ( elements[x] == null ) {
				elements[x] = o;
				break;
				}
			}
		if ( x == SET_SIZE ) { throw new IndexOutOfBoundsException(); }
		} /* end addElement() */

	public void removeElement( Object o ) {
		int x = 0;
		for( x = 0; x < SET_SIZE; x++ ) {
			if ( elements[x] == o ) {
				elements[x] = null;
				}
			}
		} /* end removeElement() */

} /* end class Set */
