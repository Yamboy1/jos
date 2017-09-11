package org.jos.program2b;

// *D! import org.jos.console3a.Console;
import org.jos.console3a.ConsoleFilter;

/**
 * This class should be useful as a base class for all your compatible programs.
 */
public interface ConsoleProgram
// *D! extends Console, Program {
		extends ConsoleFilter, Program {
	/*
	 * *D! Gets Console property. / public abstract Console getConsole();
	 */

	/*
	 * *D! Sets Console property. / public abstract void setConsole( Console v
	 * );
	 */
}
