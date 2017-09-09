package org.jos.console.impl;

import org.jos.console.Console;

public class NullConsole implements Console {
	/**
	 * Creates a console.
	 */
	public NullConsole() {
		super();
	}

	/**
	 * Prints (rather, doesn't print) a message.
	 */
	@Override
	public void println(String v) {
		// do nothing
	}
}
