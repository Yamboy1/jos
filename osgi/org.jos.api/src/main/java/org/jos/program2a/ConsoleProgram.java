package org.jos.program2a;

// *D! import com.realignsoft.consolestudio2a.Console;
import org.jos.console3a.Console;

/**
 * This class should be useful as a base class for all your compatible programs.
 */
@Deprecated
public interface ConsoleProgram extends Console, Program {
	/**
	 * Gets Console property.
	 */
	public Console getConsole();

	/**
	 * Sets Console property.
	 */
	public void setConsole(Console v);
}
