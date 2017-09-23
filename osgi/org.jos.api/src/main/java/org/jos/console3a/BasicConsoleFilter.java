package org.jos.console3a;

/**
 * This is a basic implementation of a console filter. It is most helpful when
 * providing temporary console to another object.
 *
 * <PRE>
 * public void example() {
 * 	Console c = getConsole();
 * 	ConsoleFilter cf = new BasicConsoleFilter(c);
 * 	anotherObject.setConsole(cf);
 * 	// use anotherObject.
 * 	cf.setConsole(null);
 * }
 * </PRE>
 */
public class BasicConsoleFilter implements ConsoleFilter {
	/**
	 * Creates a console filter.
	 */
	public BasicConsoleFilter() {
	}

	/**
	 * Creates a console filter, with a given console.
	 *
	 * @param v
	 *            another console
	 */
	public BasicConsoleFilter(Console v) {
		setConsole(v);
	}

	/**
	 * Gets Console property.
	 */
	public Console getConsole() {
		return console;
	}

	/**
	 * Sets Console property.
	 */
	public void setConsole(Console v) {
		console = v;
	}

	/**
	 * Prints a message.
	 *
	 * @param v
	 *            a message.
	 */
	public void println(String v) {
		if (console == null) {
			return;
		}

		console.println(v);
	}

	private Console console = null;
}
