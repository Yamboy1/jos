package org.jos.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.jos.gchii.exception1a.ExceptionTool;
import org.junit.Test;

public class ExceptionTool_Test {

	@Test
	public void test() {
		final ExceptionTool et = ExceptionTool.getTool();
		assertNotNull("exception tools is missing", et);

		final Throwable t = new Exception("this is an exception");
		assertNotNull("throwable is missing", t);

		final String mExpected = "[java.lang.Exception] this is an exception";
		final String mActual = et.format(t);
		assertEquals("expected message is not actual message", mExpected, mActual);
	}
}
