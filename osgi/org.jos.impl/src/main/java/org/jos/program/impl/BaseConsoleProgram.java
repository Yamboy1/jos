package org.jos.program.impl;

import org.jos.console.Console;

import org.jos.program.ConsoleProgram;

/**
 * This class should be useful as a base class for all your compatible programs.
 */
public abstract class BaseConsoleProgram extends BaseProgram implements ConsoleProgram {
	/**
	 * Creates a program.
	 */
	protected BaseConsoleProgram() {
		super();
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
	 * Prints a message on a console.
	 */
	public void println(String v) {
		if (console == null) {
			return;
		}

		console.println(v);
	}

	private Console console = null;
}
