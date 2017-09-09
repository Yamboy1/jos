package org.jos.program;

import java.io.InputStream;
import java.io.OutputStream;

/**
 * An I/O stream interface is necessary for the classic stream-based program.
 */
public interface IOStream {
	/**
	 * Gets InputStream property. This method returns an instance of
	 * <CODE>java.io.InputStream</CODE>.
	 */
	public InputStream getInputStream();

	/**
	 * Gets OutputStream property. This method returns an instance of
	 * <CODE>java.io.OutputStream</CODE>.
	 */
	public OutputStream getOutputStream();

	/**
	 * Closes the whole I/O Stream. Both the input stream and output stream are
	 * closed.
	 */
	public void close();
}
