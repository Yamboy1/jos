package org.jos.program2a;

/**
 * This class should be useful as a base class for all your compatible I/O
 * programs.
 */
@Deprecated
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
