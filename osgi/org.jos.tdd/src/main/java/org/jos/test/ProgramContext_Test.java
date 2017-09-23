package org.jos.test;

import static org.junit.Assert.assertNotNull;

import java.util.Enumeration;

import org.jos.program2b.ProgramContext;
import org.junit.Test;

public class ProgramContext_Test {

	@Test
	public void test() {
		ProgramContext pc = new ProgramContext() {

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

			@Override
			public int getResultCode() {
				return 0;
			}

			@Override
			public void setResultCode(int v) {
			}
		};
		assertNotNull("program context is missing", pc);
	}

}
