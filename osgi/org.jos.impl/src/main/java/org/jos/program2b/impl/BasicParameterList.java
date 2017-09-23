package org.jos.program2b.impl;

import java.util.Enumeration;

import org.jos.program2a.ParameterList;
import org.jos.uritool2a.URITool;

/**
 * A basic parameter list provides a list of parameters from a string array; and
 * a list of parameters from a query string.
 */
public final class BasicParameterList implements ParameterList {
	// static methods and fields
	private static final String SEPARATOR = "=";

	// instance methods and fields
	/**
	 * Creates a parameter list, with no parameters.
	 */
	public BasicParameterList() {
	}

	/**
	 * Parse arguments.
	 */
	public void parseArguments(String v) {
		data = QueryStringTool.getTool().getParameters(v);
	}

	/**
	 * Parse arguments, from a string array.
	 */
	public void parseArguments(String[] v) {
		if (v != null) {
			int iMax = v.length;
			data = new String[iMax];
			System.arraycopy(v, 0, data, 0, iMax);
		}
	}

	/**
	 * Gets a parameter, by index.
	 */
	public String getParameter(int index) {
		if (data == null) {
			return null;
		}

		return data[index];
	}

	/**
	 * Gets a value, by key.
	 */
	public String getValue(String key) {
		if (key == null) {
			return null;
		}

		Enumeration<String> list = getParameters();
		for (;;) {
			if (!list.hasMoreElements()) {
				return null;
			}

			String property = list.nextElement();

			int pos = property.indexOf(SEPARATOR);
			if (pos < 0) {
				continue;
			}

			String name = property.substring(0, pos);

			if (name.equals(key)) {
				return URITool.getTool().decode(property.substring(pos + 1));
			}
		}
	}

	/**
	 * Gets all parameters.
	 */
	@Override
	public Enumeration<String> getParameters() {
		return new ObjectArrayEnumeration(data);
	}

	private String[] data = null;
}
