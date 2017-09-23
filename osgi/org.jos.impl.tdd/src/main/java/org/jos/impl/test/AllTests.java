package org.jos.impl.test;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ BaseConsoleProgram_Test.class, BaseProgram_Test.class, BasicParameterList_Test.class,
		BasicProgramContext_Test.class, IndentConsole_Test.class, NullConsole_Test.class, StandardErrConsole_Test.class,
		StandardOutConsole_Test.class })
public class AllTests {

}
