package org.jos.impl.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.jos.program2b.Program;
import org.jos.program2b.ProgramContext;
import org.jos.program2b.impl.BaseProgram;
import org.jos.program2b.impl.BasicProgramContext;
import org.junit.Test;

public class BaseProgram_Test {

	@Test
	public void test() {
		final BaseProgram bp = new BaseProgram() {

			@Override
			public void run() {
				System.out.println("running base program");
			}

		};
		assertNotNull("base program is missing", bp);

		final Program p = bp;
		assertNotNull("program is missing", p);

		final Runnable r = p;
		assertNotNull("runnable is missing", r);

		/*
		 * getProgramContext()
		 */
		assertNull("program context must not be set", bp.getProgramContext());

		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pcExpected = bpc;
		assertNotNull("program context is missing", pcExpected);
		/*
		 * setProgramContext()
		 */
		bp.setProgramContext(pcExpected);
		/*
		 * getProgramContext()
		 */
		final ProgramContext pcActual = bp.getProgramContext();
		assertEquals("expected program context is not actual program context", pcExpected, pcActual);

		{
			final int rcExpected = 0;
			/*
			 * getResultCode()
			 */
			final int rcActual = bp.getResultCode();
			assertTrue("expected result code is not actual result code", rcExpected == rcActual);
		}

		{
			final int iMax = 255;
			for (int i = 0; i <= iMax; i++) {
				/*
				 * setResultCode(int)
				 */
				bp.setResultCode(i);
				/*
				 * getResultCode()
				 */
				final int rcActual = bp.getResultCode();
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
				final String vActual = bp.getValue(key);
				System.out.println("vExpected=" + vExpected + " vActual=" + vActual);
				assertTrue("expected value is not actual value", vExpected.equals(vActual));
			}
		}

		/*
		 * run()
		 */
		r.run();
	}
}
