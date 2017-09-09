package org.jos.program;

import java.util.Enumeration;

/**
 * This object provides parameters to a program. With the introduction of a
 * servlet program, it is necessary to split a parameter list reader and a
 * parameter list reader/writer. A program context implements a "parameter list"
 * interface.
 */
public interface ProgramParameterList extends ParameterList {
	/**
	 * Gets Path property.
	 */
	public abstract String getPath();

	/**
	 * Sets Path property.
	 */
	public abstract void setPath(String v);

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
	 * <P>
	 * Properties are consistently numbered from 0 through n. This corresponds
	 * exactly to the argument list from
	 * <CODE>public static void main( String[] )</CODE>. It also corresponds to
	 * the query string from a servlet. A parameter is the whole parameter in
	 * the form of name or name=value.
	 */
	public abstract String getParameter(int index);

	/**
	 * Gets parameters. Parameters are enumerated in the order in which they
	 * appear or a command line or query string.
	 * 
	 * @returns The whole parameter (name or name=value) is returned as a string
	 *          object.
	 */
	public abstract Enumeration<String> getParameters();
}
