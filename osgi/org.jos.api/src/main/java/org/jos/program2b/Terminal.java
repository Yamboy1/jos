package org.jos.program2b;

// *D! import java.util.Enumeration;

// *D! import com.realignsoft.consolestudio2a.Console;

/**
 * A terminal reads and writes lines (messages). A terminal program context
 * implements a simple terminal interface.
 */
public interface Terminal {
	/**
	 * Reads a line of text.
	 */
	public abstract String readLine();

	/**
	 * Writes a message.
	 */
	public abstract void println(String v);
}
