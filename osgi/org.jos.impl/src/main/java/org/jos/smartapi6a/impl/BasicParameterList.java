package org.jos.smartapi6a.impl;

import java.util.Enumeration;

import org.jos.program2a.ParameterList;
import org.jos.uritool2a.URITool;

/**
 * A basic parameter list provides a list of parameters from a stringarray; and
 * a list of parameters from a URI-encoded query string.
 */
class BasicParameterList implements ParameterList {
	// static methods and fields
	private static final String EQ = "=";

	/**
	 * Parameters in a query string are separates by the amperstand character.
	 */
	public static final String AMP = "&";

	// instance methods and fields
	/**
	 * Creates a parameter list, with no parameters.
	 */
	BasicParameterList() {
	}

	/**
	 * Parse arguments.
	 * 
	 * @param v
	 *            a URI-encoded query string
	 */
	public void parseArguments(String v) {
		data = getParameters(v);
	}

	/**
	 * Parse arguments, from a string array.
	 * 
	 * @param v
	 *            an array of un-encoded arguments.
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

		String separator = EQ;

		String match = key + separator;
		Enumeration<String> list = getParameters();
		for (;;) {
			if (!list.hasMoreElements()) {
				return null;
			}

			String property = list.nextElement();
			try {
				if (property.startsWith(match)) {
					return property.substring(match.length());
				}
			} catch (NullPointerException e) {
				// continue
			}
		}
	}

	/**
	 * Gets all parameters.
	 */
	public Enumeration<String> getParameters() {
		@SuppressWarnings({ "unchecked", "rawtypes" })
		Enumeration<String> r = (Enumeration) new ObjectArrayEnumeration(data);
		return r;
	}

	/**
	 * Convert a query string from
	 * 
	 * <PRE>
	 * name-1=value-1&name-2=value-2&name-3=value-3
	 * </PRE>
	 * 
	 * to a parameter list
	 * 
	 * <PRE>
	 * name-1=value-1
	 * name-2=value-2
	 * name-3=value-3
	 * </PRE>
	 */
	public String[] getParameters(String v) {

		if (v == null) {
			return null;
		}

		String separator = AMP;

		// count parameters
		int iMax = 1;
		{
			int pos = -(separator.length());
			for (;; iMax++) {
				pos = v.indexOf(separator, pos + separator.length());
				if (pos < 0) {
					break;
				}
			}
		}

		// capture parameters
		String[] buf = new String[iMax];
		for (int i = 0; i < iMax; i++) {
			int pos = v.indexOf(separator);
			if (pos < 0) {
				buf[i] = decodeParameter(v);
				break;
			}

			buf[i] = decodeParameter(v.substring(0, pos));
			v = v.substring(pos + separator.length());
		}

		return buf;
	}

	/**
	 * Decode a parameter.
	 */
	protected String decodeParameter(String p) {

		URITool t = URITool.getTool();

		String match = EQ;
		int pos = p.indexOf(match);
		if (pos < 0) {
			return t.decode(p);
		} else {
			return t.decode(p.substring(0, pos)) + match + t.decode(p.substring(pos + match.length()));
		}
	}

	private String[] data;
}
