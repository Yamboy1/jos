package org.jos.test1a;

import org.jos.console3a.Console;

public final class TestConsole implements Console {
	public TestConsole() {
		super();
	}

	/**
	 * Prints a message;
	 */
	@Override
	public void println(String message) {
		System.out.println(message);
	}
}
