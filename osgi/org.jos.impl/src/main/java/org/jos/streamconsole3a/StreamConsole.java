package org.jos.streamconsole3a;

import java.io.IOException;

import org.jos.console3a.Console;

/**
 * A stream console is-a console that usually writes messages to an output
 * stream.
 */
public interface StreamConsole extends Console {
	/**
	 * Flushes a stream console. Flushing a stream console will force messages
	 * in a buffer to be written to disk.
	 * 
	 * @exception IOException
	 *                This exception might be thrown by a stream-based console
	 *                when a stream cannot be flushed.
	 */
	public void flush() throws IOException;

	/**
	 * Closes a stream console.
	 * 
	 * @exception IOException
	 *                This exception might be thrown by a stream-based console
	 *                when a stream cannot be closed.
	 */
	public void close() throws IOException;
}
