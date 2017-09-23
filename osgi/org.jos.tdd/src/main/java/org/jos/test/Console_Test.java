package org.jos.test;

import static org.junit.Assert.assertNotNull;

import org.jos.console3a.Console;
import org.junit.Test;

public class Console_Test {

	@Test
	public void test() {
		final Console c = new Console() {
			@Override
			public void println(String v) {
				System.out.println("[PRINTLN] " + v);
			}
		};
		assertNotNull("console is missing", c);

		c.println("sending message to console");
	}
}
