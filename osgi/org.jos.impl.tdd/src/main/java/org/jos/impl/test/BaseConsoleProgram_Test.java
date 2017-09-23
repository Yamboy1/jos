package org.jos.impl.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.jos.console3a.Console;
import org.jos.console3a.impl.StandardOutConsole;
import org.jos.program2b.ConsoleProgram;
import org.jos.program2b.Program;
import org.jos.program2b.ProgramContext;
import org.jos.program2b.impl.BaseConsoleProgram;
import org.jos.program2b.impl.BasicProgramContext;
import org.junit.Test;

public class BaseConsoleProgram_Test {

	@Test
	public void test() {
		final BaseConsoleProgram bcp = new BaseConsoleProgram() {

			@Override
			public void run() {
				println("running base console program");
			}

		};
		assertNotNull("base program is missing", bcp);

		final ConsoleProgram cp = bcp;
		assertNotNull("console program is missing", cp);

		final Program p = cp;
		assertNotNull("program is missing", p);

		final Runnable r = p;
		assertNotNull("runnable is missing", r);

		/*
		 * getProgramContext()
		 */
		assertNull("program context must not be set", bcp.getProgramContext());

		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pcExpected = bpc;
		assertNotNull("program context is missing", pcExpected);
		/*
		 * setProgramContext()
		 */
		bcp.setProgramContext(pcExpected);
		/*
		 * getProgramContext()
		 */
		final ProgramContext pcActual = bcp.getProgramContext();
		assertEquals("expected program context is not actual program context", pcExpected, pcActual);

		{
			final int rcExpected = 0;
			/*
			 * getResultCode()
			 */
			final int rcActual = bcp.getResultCode();
			assertTrue("expected result code is not actual result code", rcExpected == rcActual);
		}

		{
			final int iMax = 255;
			for (int i = 0; i <= iMax; i++) {
				/*
				 * setResultCode(int)
				 */
				bcp.setResultCode(i);
				/*
				 * getResultCode()
				 */
				final int rcActual = bcp.getResultCode();
				assertTrue("expected result code is not actual result code", i == rcActual);
			}
		}

		{
			final String[] args = { "parameter-0=value-0", "parameter-1=value-1", "parameter-2=value-2" };
			bpc.parseArguments(args);
			final int iMax = args.length;
			for (int i = 0; i < iMax; i++) {
				final String vExpected = "value-" + i;
				final String key = "parameter-" + i;
				/*
				 * getValue(String)
				 */
				final String vActual = bcp.getValue(key);
				System.out.println("vExpected=" + vExpected + " vActual=" + vActual);
				assertTrue("expected value is not actual value", vExpected.equals(vActual));
			}
		}

		{
			assertNull("console should not be set", bcp.getConsole());
		}

		{
			final Console cExpected = new StandardOutConsole();
			/*
			 * setConsole(Console)
			 */
			bcp.setConsole(cExpected);
			/*
			 * getConsole()
			 */
			final Console cActual = bcp.getConsole();
			assertEquals("expected console is not actual console", cExpected, cActual);
		}

		/*
		 * run()
		 */
		r.run();
	}
}
