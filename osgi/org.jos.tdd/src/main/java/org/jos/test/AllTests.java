package org.jos.test;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ BaseException_Test.class, Console_Test.class, ConsoleFilter_Test.class, ConsoleProgram_Test.class,
		DebugProperty_Test.class, Environment_Test.class, ExceptionTool_Test.class, ParameterList_Test.class,
		Program_Test.class, ProgramContext_Test.class })
public class AllTests {

}
