package org.jos.program;

/**
 * This class should be useful as a base class for all your compatible I/O
 * programs.
 */
public interface IOStreamProgram extends IOStream, Program {
	/**
	 * Gets IOStream property.
	 */
	public IOStream getIOStream();

	/**
	 * Sets IOStream property.
	 */
	public void setIOStream(IOStream v);
}
