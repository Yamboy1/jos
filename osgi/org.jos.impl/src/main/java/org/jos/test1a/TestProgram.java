package org.jos.test1a;

import org.jos.console3a.Console;

import org.jos.program2b.ConsoleProgram;
import org.jos.program2b.ProgramContext;

public abstract class TestProgram implements ConsoleProgram {
	private Console console;
	private ProgramContext ctx;

	/**
	 * Creates a program.
	 */
	protected TestProgram() {
		super();
	}

	/**
	 * Runs a program.
	 */
	@Override
	public final void run() {
		try {
			getProgramContext().setResultCode(0); // success, by convention
			runTest();
		} catch (Throwable e) {
			getProgramContext().setResultCode(1); // exception failure, by
													// convention
			println("[" + e.getClass().getName() + "] " + e.getMessage());
		}
	}

	protected abstract void runTest() throws TestException;

	/**
	 * Gets Console property.
	 * 
	 * @return
	 */
	public final Console getConsole() {
		return console;
	}

	/**
	 * Sets Console property.
	 * 
	 * @param c
	 */
	public final void setConsole(Console c) {
		console = c;
	}

	/**
	 * Gets ProgramContext property.
	 */
	@Override
	public final ProgramContext getProgramContext() {
		return ctx;
	}

	/**
	 * Sets ProgramContext property.
	 */
	@Override
	public final void setProgramContext(ProgramContext v) {
		ctx = v;
	}

	/**
	 * Gets Value[] property.
	 */
	protected final String getValue(String key) {
		return getProgramContext().getValue(key);
	}

	/**
	 * Prints a message
	 * 
	 * @param message
	 *            a message
	 */
	public final void println(String message) {
		if (console == null) {
			return;
		}

		console.println(message);
	}

	/**
	 * Creates a test exception.
	 */
	protected TestException _TEX(String message) {
		return new TestException(message);
	}

	/**
	 * Prints an exception (Throwable).
	 */
	protected void _print(Throwable e) {
		if (e == null) {
			return;
		}

		println("[" + e.getClass().getName() + "] " + e.getMessage());
	}
}
