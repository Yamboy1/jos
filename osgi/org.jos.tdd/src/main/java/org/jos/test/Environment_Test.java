package org.jos.test;

import static org.junit.Assert.assertNotNull;

import org.jos.program2c.Environment;
import org.junit.Test;

public class Environment_Test {

	@Test
	public void test() {
		final Environment e = new Environment() {

			@Override
			public String getAttribute(String key) {
				return null;
			}

			@Override
			public void setAttribute(String key, Object value) {
			}
		};
		assertNotNull("environment is missing", e);
	}
}
