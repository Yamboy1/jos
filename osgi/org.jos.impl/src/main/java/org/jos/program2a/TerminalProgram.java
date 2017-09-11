package org.jos.program2a;

/**
 * This class should be useful as a base class for all your compatible programs.
 */
@Deprecated
public interface TerminalProgram extends Terminal, Program {
	/**
	 * Gets Terminal property.
	 */
	public Terminal getTerminal();

	/**
	 * Sets Terminal property.
	 */
	public void setTerminal(Terminal v);
}
