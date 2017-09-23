package org.jos.impl.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Enumeration;

import org.jos.program2a.ParameterList;
import org.jos.program2b.impl.BasicParameterList;
import org.junit.Test;

public class BasicParameterList_Test {

	@Test
	public void testParseArgs() {
		final BasicParameterList bpl = new BasicParameterList();
		assertNotNull("basic parameter list is missing", bpl);

		final ParameterList pl = bpl;
		assertNotNull("parameter list is missing", pl);

		{
			/*
			 * parseArguments(String[])
			 */
			final String[] args = { "parameter-0", "parameter-1", "parameter-2" };
			pl.parseArguments(args);

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
		final BasicParameterList bpl = new BasicParameterList();
		assertNotNull("basic parameter list is missing", bpl);

		final ParameterList pl = bpl;
		assertNotNull("parameter list is missing", pl);

		final String[] args = { "parameter-0=value-0", "parameter-1=value-1", "parameter-2=value-2" };
		/*
		 * parseArguments(String[])
		 */
		pl.parseArguments(args);

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
		final BasicParameterList bpl = new BasicParameterList();
		assertNotNull("basic parameter list is missing", bpl);

		final ParameterList pl = bpl;
		assertNotNull("parameter list is missing", pl);

		final String queryString = "parameter-0&parameter-1&parameter-2";
		/*
		 * parseArguments(String)
		 */
		pl.parseArguments(queryString);

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
		final BasicParameterList bpl = new BasicParameterList();
		assertNotNull("basic parameter list is missing", bpl);

		final ParameterList pl = bpl;
		assertNotNull("parameter list is missing", pl);

		{
			final String queryString = "parameter-0=value-0&parameter-1=value-1&parameter-2=value-2";
			/*
			 * parseArguments(String)
			 */
			pl.parseArguments(queryString);

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
