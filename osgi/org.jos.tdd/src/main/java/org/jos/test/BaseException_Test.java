package org.jos.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

import org.jos.gchii.exception1a.BaseException;
import org.junit.Test;

public class BaseException_Test {

	@Test
	public void testConstructor1() {
		final ConcreteException ce = new ConcreteException();
		assertNotNull("concrete exception is missing", ce);

		final BaseException be = ce;
		assertNotNull("base exception is missing", be);

		final Exception e = be;
		assertNotNull("exception is missing", e);

		final Throwable t = e;
		assertNotNull("throwable is missing", t);

		assertNull("message should not be set because constructed with default constructor", t.getMessage());

		final String mExpected = "constructor 1 message";
		be.setMessage(mExpected);
		final String mActual = t.getMessage();
		assertEquals("expected message is not actual message", mExpected, mActual);
	}

	@Test
	public void testConstructor2() {
		final String mExpected = "constructor 2 message";
		final ConcreteException ce = new ConcreteException(mExpected);
		assertNotNull("concrete exception is missing", ce);

		final BaseException be = ce;
		assertNotNull("base exception is missing", be);

		final Exception e = be;
		assertNotNull("exception is missing", e);

		final Throwable t = e;
		assertNotNull("throwable is missing", t);

		final String mActual = t.getMessage();
		assertEquals("expected message is not actual message", mExpected, mActual);
	}
}

final class ConcreteException extends BaseException {
	private static final long serialVersionUID = 1L;

	ConcreteException() {
		super();
	}

	ConcreteException(final String message) {
		super(message);
	}
};