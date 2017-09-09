package org.jos.program.impl;

import java.util.Enumeration;

import org.jos.program.ParameterList;

import org.jos.uritool.URITool;

/**
 * A basic parameter list provides a list of parameters from a string array; and
 * a list of parameters from a query string.
 */
final class BasicParameterList implements ParameterList {
	private static final String SEPARATOR = "=";

	private String[] data = null;

	/**
	 * Creates a parameter list, with no parameters.
	 */
	BasicParameterList() {
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

			String property = (String) list.nextElement();

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
	 * Gets a values, by key.
	 */
	public String[] getValues(final String key) {
		if (key == null) {
			return null;
		}

		String[] result = new String[]{};
		
		Enumeration<String> list = getParameters();
		for (;;) {
			if (!list.hasMoreElements()) {
				return result;
			}

			String property = (String) list.nextElement();

			int pos = property.indexOf(SEPARATOR);
			if (pos < 0) {
				continue;
			}

			String name = property.substring(0, pos);

			if (name.equals(key)) {
				int i = result.length;
				final String[] t = new String[i + 1];
				System.arraycopy(result, 0, t, 0, i);
				t[i] = URITool.getTool().decode(property.substring(pos + 1));
				result = t;
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
}
