package org.jos.impl.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Enumeration;

import org.jos.program2b.ParameterList;
import org.jos.program2b.ProgramContext;
import org.jos.program2b.impl.BasicProgramContext;
import org.junit.Test;

public class BasicProgramContext_Test {

	/**
	 * A program can get its own program-context; but, it should not easily
	 * parse its own parameters and set its own path.
	 */
	@Test
	public void testPath() {
		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pc = bpc;
		assertNotNull("program context is missing", pc);

		final ParameterList pl = pc;
		assertNotNull("console is missing", pl);

		final String pExpected = "path-1";
		bpc.setPath(pExpected);
		final String pActual = pl.getPath();
		assertTrue("expected path is not actual path", pExpected.equals(pActual));
	}

	@Test
	public void testParseArgs() {
		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pc = bpc;
		assertNotNull("program context is missing", pc);

		final ParameterList pl = pc;
		assertNotNull("parameter list is missing", pl);

		{
			/*
			 * parseArguments(String[])
			 */
			final String[] args = { "parameter-0", "parameter-1", "parameter-2" };
			bpc.parseArguments(args);

			final int iMax = args.length;
			for (int i = 0; i < iMax; i++) {
				/*
				 * getParameter(int)
				 */
				final String pActual = pl.getParameter(i);
				assertTrue("expected parameter is not actual parameter " + i, args[i].equals(pActual));
			}
		}
	}

	@Test
	public void testValueArgs() {
		final String prefix = "[testValueArgs]";
		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pc = bpc;
		assertNotNull("program context is missing", pc);

		final ParameterList pl = pc;
		assertNotNull("parameter list is missing", pl);

		final String[] args = { "parameter-0=value-0", "parameter-1=value-1", "parameter-2=value-2" };
		/*
		 * parseArguments(String[])
		 */
		bpc.parseArguments(args);

		{
			final int iMax = args.length;
			for (int i = 0; i < iMax; i++) {
				final String vExpected = "value-" + i;
				final String key = "parameter-" + i;
				/*
				 * getValue(String)
				 */
				final String vActual = pl.getValue(key);
				System.out.println(prefix + " vActual=" + vActual);
				assertTrue("expected parameter is not actual parameter " + i, vExpected.equals(vActual));
			}
		}

		{
			/*
			 * getParameters()
			 */
			final Enumeration<String> list = pl.getParameters();
			assertNotNull("parameters enumeration is missing", list);

			int i = 0;
			for (;;) {
				if (!list.hasMoreElements()) {
					break;
				}

				final String pExpected = args[i];
				final String pActual = list.nextElement();

				System.out.println(prefix + " vActual=" + pActual);
				assertTrue("expected parameter is not actual parameter " + i, pExpected.equals(pActual));

				i++;
			}
		}
	}

	@Test
	public void testParseQueryString() {
		final String prefix = "[testParseQueryString]";

		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pc = bpc;
		assertNotNull("program context is missing", pc);

		final ParameterList pl = pc;
		assertNotNull("parameter list is missing", pl);

		final String queryString = "parameter-0&parameter-1&parameter-2";
		/*
		 * parseArguments(String)
		 */
		bpc.parseArguments(queryString);

		final int iMax = 3;
		for (int i = 0; i < iMax; i++) {
			final String pExpected = "parameter-" + i;
			System.out.println(prefix + " pExpected=" + pExpected);
			/*
			 * getParameter(int)
			 */
			final String pActual = pl.getParameter(i);
			System.out.println(prefix + " pActual=" + pActual);
			assertTrue("expected parameter is not actual parameter " + i, pExpected.equals(pActual));
		}
	}

	@Test
	public void testValueQueryString() {
		final String prefix = "[testValueQueryString]";

		final BasicProgramContext bpc = new BasicProgramContext();
		assertNotNull("basic program context is missing", bpc);

		final ProgramContext pc = bpc;
		assertNotNull("program context is missing", pc);

		final ParameterList pl = pc;
		assertNotNull("parameter list is missing", pl);

		{
			final String queryString = "parameter-0=value-0&parameter-1=value-1&parameter-2=value-2";
			/*
			 * parseArguments(String)
			 */
			bpc.parseArguments(queryString);

			final int iMax = 3;
			for (int i = 0; i < iMax; i++) {
				final String vExpected = "value-" + i;
				final String key = "parameter-" + i;
				/*
				 * getValue(String)
				 */
				final String vActual = pl.getValue(key);
				System.out.println(prefix + " vActual=" + vActual);
				assertTrue("expected parameter is not actual parameter " + i, vExpected.equals(vActual));
			}
		}

		{
			/*
			 * getParameters()
			 */
			final Enumeration<String> list = pl.getParameters();
			assertNotNull("parameters enumeration is missing", list);

			int i = 0;
			for (;;) {
				if (!list.hasMoreElements()) {
					break;
				}

				final String pExpected = "parameter-" + i + "=value-" + i;
				final String pActual = list.nextElement();

				System.out.println(prefix + " vActual=" + pActual);
				assertTrue("expected parameter is not actual parameter " + i, pExpected.equals(pActual));

				i++;
			}
		}
	}
}
