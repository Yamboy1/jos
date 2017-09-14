package org.jos.program2a;

/**
 * A terminal reads and writes lines (messages). A terminal program context
 * implements a simple terminal interface.
 */
@Deprecated
public interface Terminal {
	/**
	 * Reads a line of text.
	 */
	public abstract String readLine();

	/**
	 * Writes a message.
	 */
	public abstract void println(String v);
}
