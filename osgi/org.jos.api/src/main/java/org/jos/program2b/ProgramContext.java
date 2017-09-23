package org.jos.program2b;

/**
 * A program has-an program context. The program context is given to a program
 * and contains parameters and a result code.
 *
 * Parameters to a program do not have to be literally implemented as String[].
 * This object contains an equivalent to String[] args for traditional Java
 * applications.
 */
public interface ProgramContext extends ParameterList {
	/**
	 * Gets ResultCode property.
	 */
	public int getResultCode();

	/**
	 * Sets ResultCode property.
	 *
	 * A result code of zero (0) is used to indicate no failure. A non-zero
	 * result code indicates a failure of some kind. There are no universal set
	 * of result codes; they are program-specific. You can use any convention
	 * you want when developing each program.
	 *
	 * A program context must provide a default value for result code,
	 * preferrably zero (0).
	 */
	public void setResultCode(int v);
}
