package org.jos.program.impl;

/**
 * This tool provides a few utility methods to parse a URI query string.
 */
final class QueryStringTool {
	// static methods and fields
	/**
	 * Parameters in a query string are separates by the amperstand character.
	 */
	private static final String SEPARATOR = "&";

	private static final QueryStringTool tool = new QueryStringTool();

	/**
	 * Gets default tool.
	 */
	static QueryStringTool getTool() {
		return tool;
	}

	// instance methods and fields
	protected QueryStringTool() {
	}

	/**
	 * Convert a parameter list from
	 * 
	 * <PRE>
	 * name-1=value-1
	 * name-2=value-2
	 * name-3=value-3
	 * </PRE>
	 * 
	 * to a query string
	 * 
	 * <PRE>
	 * name-1=value-1&name-2=value-2&name-3=value-3
	 * </PRE>
	 */
	public String getQueryString(String[] v) {
		if (v == null) {
			return null;
		}

		int iMax = v.length;
		StringBuffer buffer = new StringBuffer();
		buffer.append(v[0]);
		for (int i = 1; i < iMax; i++) {
			buffer.append(SEPARATOR + v[i]);
		}
		return buffer.toString();
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

		int iMax = 1;
		int pos = -1;
		for (;; iMax++) {
			pos = v.indexOf(SEPARATOR, pos + SEPARATOR.length());
			if (pos < 0) {
				break;
			}
		}

		String[] buf = new String[iMax];
		for (int i = 0; i < iMax; i++) {
			pos = v.indexOf(SEPARATOR);
			if (pos < 0) {
				buf[i] = v;
				break;
			}

			buf[i] = v.substring(0, pos);
			v = v.substring(pos + SEPARATOR.length());
		}

		return buf;
	}
}
