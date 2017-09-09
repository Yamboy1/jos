package org.jos.console;

/**
 * A console filter is-a console and has-a console.
 */
public interface ConsoleFilter extends Console {
	/**
	 * Gets Console property.
	 */
	public Console getConsole();

	/**
	 * Sets Console property.
	 */
	public void setConsole(Console v);
}
