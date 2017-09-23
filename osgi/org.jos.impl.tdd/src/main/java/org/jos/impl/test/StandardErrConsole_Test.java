package org.jos.impl.test;

import static org.junit.Assert.assertNotNull;

import org.jos.console3a.Console;
import org.jos.console3a.impl.StandardErrConsole;
import org.junit.Test;

public class StandardErrConsole_Test {

	@Test
	public void test() {
		final StandardErrConsole soc = new StandardErrConsole();
		assertNotNull("standard out console is missing", soc);

		final Console c = soc;
		assertNotNull("console is missing", c);

		c.println("message to standard error stream");
	}
}
