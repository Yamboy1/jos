package org.jos.program2a;

import java.util.Enumeration;

/**
 * A program context implements a "parameter list" interface.
 */
public abstract interface ParameterList {
	// instance methods and fields
	/**
	 * Parse arguments, from query string.
	 */
	public abstract void parseArguments(String v);

	/**
	 * Parse arguments, from string array.
	 */
	public abstract void parseArguments(String[] v);

	/**
	 * Gets Parameter[] property.
	 *
	 * Properties are consistently numbered from 0 through n. This corresponds
	 * exactly to the argument list from
	 * <CODE>public static void main( String[] )</CODE>. It also corresponds to
	 * the query string from a servlet. A parameter is the whole parameter in
	 * the form of name or name=value.
	 */
	public abstract String getParameter(int index);

	/**
	 * Gets Value[] property.
	 *
	 * For those parameters in the form of name=value, this method uses name to
	 * find a property and return its value.
	 *
	 * For command line parameters, the key includes dashes (-) and slashes (/).
	 */
	public abstract String getValue(String key);

	/**
	 * Gets parameters.
	 *
	 * Parameters are enumerated in the order in which they appear or a command
	 * line or query string.
	 *
	 * @returns The whole parameter (name or name=value) is returned as a string
	 *          object.
	 */
	public abstract Enumeration<String> getParameters();
}
