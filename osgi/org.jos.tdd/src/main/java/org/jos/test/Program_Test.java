package org.jos.test;

import static org.junit.Assert.assertNotNull;

import org.jos.program2b.Program;
import org.jos.program2b.ProgramContext;
import org.junit.Test;

public class Program_Test {

	@Test
	public void test() {
		final Program p = new Program() {

			@Override
			public void run() {
			}

			@Override
			public ProgramContext getProgramContext() {
				return null;
			}

			@Override
			public void setProgramContext(ProgramContext v) {
			}
			
		};
		assertNotNull("program is missing", p);
		
		final Runnable r = p;
		assertNotNull("runnable is missing", r);
	}
}
