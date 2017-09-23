package org.jos.console3a.impl;

import org.jos.console3a.BasicConsoleFilter;
import org.jos.console3a.Console;

/**
 * This class is an example of a console filter. A console filter is a console
 * and has one or more consoles of its own. The indent console filter adds
 * indentation to console.
 */
public class IndentConsole extends BasicConsoleFilter {
	/**
	 * Creates a console.
	 */
	public IndentConsole() {
		this(null);
	}

	/**
	 * Creates a console, a console filter for another console.
	 *
	 * @param other
	 *            any other <CODE>jose.Console</CODE>
	 */
	public IndentConsole(Console other) {
		super(other);
		setLevel(0);
		setSpacerSize(2);
	}

	/**
	 * Get Level property.
	 */
	public int getLevel() {
		return indentLevel;
	}

	/**
	 * Set Level property. The indentation level is from 0 to MAX. 0 is no
	 * indentation at all. 1 is smallest indentation and MAX is maximum
	 * indentation.
	 *
	 * @param v
	 *            an integer from 0 to MAX
	 */
	public void setLevel(int v) {
		indentLevel = v;
	}

	/**
	 * Indent one level. Adds 1 to the indent level.
	 */
	public void indent() {
		indentLevel++;
	}

	/**
	 * Outdent out level. Subtracts 1 from indent level.
	 */
	public void outdent() {
		indentLevel--;
	}

	/**
	 * Prints a line of text (String).
	 */
	public void println(String v) {
		super.println(getSpacer() + v);
	}

	/**
	 * Calculates the whitespace for current level.
	 */
	protected String getSpacer() {
		StringBuffer buffer = new StringBuffer();
		for (int i = 0; i < indentLevel; i++) {
			buffer.append(spacerUnit);
		}
		return buffer.toString();
	}

	/**
	 * Gets SpacerSize property.
	 */
	public int getSpacerSize() {
		if (spacerUnit == null) {
			return 0;
		}

		return spacerUnit.length();
	}

	/**
	 * Sets SpacerSize property.
	 */
	public void setSpacerSize(int v) {
		if (v < 1) {
			return;
		}

		char[] buf = new char[v];
		for (int i = 0; i < v; i++) {
			buf[i] = ' ';
		}
		spacerUnit = new String(buf);
	}

	/**
	 * Level property.
	 */
	private int indentLevel;

	/**
	 * SpacerUnit property.
	 */
	private String spacerUnit;
}
