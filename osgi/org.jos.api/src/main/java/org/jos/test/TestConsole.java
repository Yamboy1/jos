package org.jos.test;

import org.jos.console.Console;

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
