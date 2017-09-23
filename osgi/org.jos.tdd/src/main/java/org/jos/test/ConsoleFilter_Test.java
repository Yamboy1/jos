package org.jos.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.jos.console3a.BasicConsoleFilter;
import org.jos.console3a.Console;
import org.jos.console3a.ConsoleFilter;
import org.junit.Test;

public class ConsoleFilter_Test {

	@Test
	public void test() {
		final BasicConsoleFilter bcf = new BasicConsoleFilter();
		assertNotNull("basic console filter is missing", bcf);
		
		final ConsoleFilter cf = bcf;
		assertNotNull("console filter is missing", cf);
		
		final Console c = cf;
		assertNotNull("console is missing", c);

		c.println("this message should not be printed because console property has not yet been set");

		final Console cExpected = new Console() {

			@Override
			public void println(String v) {
				System.out.println("[ConsoleFilter_Test.Console println] " + v);
			}
		};
		assertNotNull("expected console is missing", cExpected);
		
		cf.setConsole(cExpected);
		
		final Console cActual = cf.getConsole();
		assertNotNull("actual console is missing", cActual);
		assertEquals("expected console is not actual console", cExpected, cActual);
		
		c.println("sending message to console");
	}
}
