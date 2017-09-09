package org.jos.program;

/**
 * Any object that implements this interface is an executable program.
 */
public interface Program extends Runnable {
	/**
	 * Gets ProgramContext property.
	 */
	public abstract ProgramContext getProgramContext();

	/**
	 * The host environment provices a program context.
	 */
	public abstract void setProgramContext(ProgramContext v);
}
