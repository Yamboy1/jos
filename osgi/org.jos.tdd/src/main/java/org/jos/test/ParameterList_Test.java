package org.jos.test;

import static org.junit.Assert.assertNotNull;

import java.util.Enumeration;

import org.jos.program2b.ParameterList;
import org.junit.Test;

public class ParameterList_Test {

	@Test
	public void test() {
		final ParameterList pl = new ParameterList() {

			@Override
			public String getPath() {
				return null;
			}

			@Override
			public String getParameter(int index) {
				return null;
			}

			@Override
			public String getValue(String key) {
				return null;
			}

			@Override
			public String[] getValues(String v) {
				return null;
			}

			@Override
			public Enumeration<String> getParameters() {
				return null;
			}

		};
		assertNotNull("parameter list is missing", pl);
	}
}
