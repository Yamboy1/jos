package org.jos.program2a;

/**
 * Any object that implements this interface is an executable program.
 */
@Deprecated
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
