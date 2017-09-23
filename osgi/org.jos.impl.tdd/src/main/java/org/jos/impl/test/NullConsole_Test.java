package org.jos.impl.test;

import static org.junit.Assert.assertNotNull;

import org.jos.console3a.Console;
import org.jos.console3a.impl.NullConsole;
import org.junit.Test;

public class NullConsole_Test {

	@Test
	public void test() {
		final NullConsole nc = new NullConsole();
		assertNotNull("null console is missing", nc);

		final Console c = nc;
		assertNotNull("console is missing", c);

		c.println("this message should not be printed!");
	}
}
