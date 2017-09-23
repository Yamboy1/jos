package org.jos.impl.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import org.jos.console3a.BasicConsoleFilter;
import org.jos.console3a.Console;
import org.jos.console3a.ConsoleFilter;
import org.jos.console3a.impl.IndentConsole;
import org.junit.Test;

public class IndentConsole_Test {

	@Test
	public void test() {
		final IndentConsole ic = new IndentConsole();
		assertNotNull("null console is missing", ic);

		final BasicConsoleFilter bcf = ic;
		assertNotNull("basic console filter is missing", bcf);

		final ConsoleFilter cf = bcf;
		assertNotNull("console filter is missing", cf);

		final Console c = cf;
		assertNotNull("console is missing", c);

		c.println("this message should not be printed because console has not been set");

		final Console cExpected = new Console() {

			@Override
			public void println(String v) {
				System.out.println("[OTHER]" + v);
			}
		};
		cf.setConsole(cExpected);
		final Console cActual = cf.getConsole();
		assertEquals("expected console is not actual console", cExpected, cActual);

		c.println("message 1");

		{
			final int ssExpected = 2;
			final int ssActual = ic.getSpacerSize();
			assertTrue("expected spacer size is not actual spacer size", ssExpected == ssActual);
		}

		{
			final int lExpected = 0;
			final int lActual = ic.getLevel();
			assertTrue("expected level is not actual level", lExpected == lActual);
		}

		c.println("message indent 0");

		final int jMax = 8;
		for (int j = 1; j <= jMax; j++) {
			ic.setSpacerSize(j);
			final int ssActual = ic.getSpacerSize();
			assertTrue("expected spacer size is not actual spacer size", j == ssActual);

			final int iMax = 80;
			for (int i = 0; i <= iMax; i++) {
				ic.setLevel(i);
				final int lActual = ic.getLevel();
				assertTrue("expected level is not actual level", i == lActual);

				c.println("message indent " + i);
			}
		}
	}
}
