package org.jos.console3a.impl;

import org.jos.console3a.Console;

public final class NullConsole implements Console {
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
