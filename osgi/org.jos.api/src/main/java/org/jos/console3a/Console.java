package org.jos.console3a;

/**
 * This interface provides an API for all kinds of consoles.
 */
public interface Console {
	/**
	 * Prints a message. While each message is typically printed on a separate
	 * line, there is no assumption about what a console might do with a
	 * message. It could be thrown away, saved to disk or passed along to
	 * another console.
	 * 
	 * @param v
	 *            a message
	 */
	public void println(String v);
}
