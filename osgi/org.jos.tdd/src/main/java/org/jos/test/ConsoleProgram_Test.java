package org.jos.test;

import static org.junit.Assert.assertNotNull;

import org.jos.console3a.Console;
import org.jos.program2b.ConsoleProgram;
import org.jos.program2b.Program;
import org.jos.program2b.ProgramContext;
import org.junit.Test;

public class ConsoleProgram_Test {

	@Test
	public void test() {
		final ConsoleProgram cp = new ConsoleProgram() {

			@Override
			public Console getConsole() {
				return null;
			}

			@Override
			public void setConsole(Console v) {
			}

			@Override
			public void println(String v) {
			}

			@Override
			public ProgramContext getProgramContext() {
				return null;
			}

			@Override
			public void setProgramContext(ProgramContext v) {
			}

			@Override
			public void run() {
			}

		};
		assertNotNull("console program is missing", cp);

		final Program p = cp;
		assertNotNull("program is missing", p);

		final Runnable r = p;
		assertNotNull("runnable is missing", r);
	}
}
