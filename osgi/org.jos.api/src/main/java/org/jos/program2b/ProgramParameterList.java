package org.jos.program2b;

import java.util.Enumeration;

/**
 * This object provides parameters to a program. With the introduction of a
 * servlet program, it is necessary to split a parameter list reader and a
 * parameter list reader/writer. A program context implements a "parameter list"
 * interface.
 */
public interface ProgramParameterList extends ParameterList {
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

	/*
	 * *D! Gets Value[] property. <P>For those parameters in the form of
	 * name=value, this method uses name to find a property and return its
	 * value. <P>For command line parameters, the key includes dashes (-) and
	 * slashes (/). / public abstract String getValue( String key );
	 */

	/**
	 * Gets parameters.
	 * <P>
	 * Parameters are enumerated in the order in which they appear or a command
	 * line or query string.
	 * 
	 * @returns The whole parameter (name or name=value) is returned as a string
	 *          object.
	 */
	@SuppressWarnings("rawtypes")
	public abstract Enumeration getParameters();
}
