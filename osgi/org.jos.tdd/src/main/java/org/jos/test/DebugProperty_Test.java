package org.jos.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import org.jos.debug1a.DebugProperty;
import org.junit.Test;

public class DebugProperty_Test {

	@Test
	public void test() {
		final DebugProperty dp = new DebugProperty() {
			boolean debugFlag = false;

			@Override
			public boolean isDebug() {
				return debugFlag;
			}

			@Override
			public void setDebug(boolean flag) {
				debugFlag = flag;
			}

			@Override
			public void debug(String message) {
				if (debugFlag) {
					System.out.println("[DEBUG] " + message);
				}
			}

		};
		assertNotNull("debug property is missing", dp);

		dp.debug("this message should not be printed because debug property has not yet been set");

		boolean dExpected = true;
		dp.setDebug(dExpected);
		boolean dActual = dp.isDebug();
		assertTrue("expected debug flag is not actual debug flag", dExpected == dActual);

		dp.debug("sending message to debug property");
	}
}
