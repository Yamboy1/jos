package org.jos.console3a.impl;

import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import org.jos.console3a.Console;

/**
 * This implementation provides a consistent output to the standard output
 * stream at the time of construction. It continues to output to the original
 * standard out, even if the standard output is set to some other stream.
 */
public class StandardErrConsole implements Console {
	private PrintWriter writer = null;

	/**
	 * Creates a console.
	 */
	public StandardErrConsole() {
		super();
		writer = new PrintWriter(new OutputStreamWriter(System.err));
	}

	/**
	 * Prints a message.
	 */
	@Override
	public void println(String v) {
		writer.println(v);
		writer.flush();
	}
}
