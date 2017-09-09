package org.jos.streamconsole;

import java.io.IOException;
import java.io.PrintStream;
import java.io.OutputStream;

/**
 * This is a basic implementation of a stream console. It converts any
 * OutputStream (or PrintStream) to a stream console.
 */
public class BasicStreamConsole implements StreamConsole {
	/**
	 * Creates a console.
	 */
	public BasicStreamConsole() {
	}

	/**
	 * Creates a console, with a given OutputStream (or PrintStream).
	 * 
	 * @param v
	 *            an output stream
	 */
	public BasicStreamConsole(OutputStream v) {
		setOutputStream(v);
	}

	/**
	 * Gets OutputStream property.
	 */
	public OutputStream getOutputStream() {
		return output;
	}

	/**
	 * Sets OutputStream property.
	 */
	public void setOutputStream(OutputStream v) {
		if (v == null) {
			return;
		}

		if (v instanceof PrintStream) {
			output = (PrintStream) v;
			return;
		}

		output = new PrintStream(v);
	}

	/**
	 * Prints a message.
	 * 
	 * @param v
	 *            a message
	 */
	public void println(String v) {
		if (output == null) {
			return;
		}

		output.println(v);
	}

	/**
	 * Flushes a stream console. Flushing a stream console will force messages
	 * in a buffer to be written to disk.
	 * 
	 * @exception IOException
	 *                This exception might be thrown by a stream-based console
	 *                when a stream cannot be flushed.
	 */
	public void flush() throws IOException {
		if (output == null) {
			return;
		}

		output.flush();
	}

	/**
	 * Closes a stream console.
	 * 
	 * @exception IOException
	 *                This exception might be thrown by a stream-based console
	 *                when a stream cannot be closed.
	 */
	public void close() throws IOException {
		if (output == null) {
			return;
		}

		output.close();
	}

	private PrintStream output = null;
}
