package org.jos.uritool2a;

/**
 * This tool encodes/decodes a string using URI encoding.
 */
public class BaseURITool {
	// static methods and fields
	private static final int SP = 32;

	// instance methods and fields.
	/**
	 * Creates a URITool.
	 */
	protected BaseURITool() {
	}

	/**
	 * Decodes a string.
	 * 
	 * @param v
	 *            encoded value
	 */
	public String decode(String v) {
		return decodePercent(decodePlus(v));
	}

	protected String decodePlus(String v) {
		if (v == null) {
			return null;
		}

		int pos = v.indexOf("+");
		if (pos < 0) {
			return v;
		}

		String s = decodePlus(v.substring(pos + 1));
		return v.substring(0, pos) + " " + s;
	}

	protected String decodePercent(String v) {
		if (v == null) {
			return null;
		}

		int pos = v.indexOf("%");
		if (pos < 0) {
			return v;
		}

		String i = v.substring(pos + 1, pos + 3);

		char ch;
		try {
			ch = (char) Integer.valueOf(i, 16).intValue();
		} catch (NumberFormatException e) {
			return null;
		}

		String s = decodePercent(v.substring(pos + 3));
		return v.substring(0, pos) + ch + s;
	}

	/**
	 * Decodes a string as a byte array.
	 */
	public byte[] decodeBytes(String v) {
		if (v == null) {
			return null;
		}

		int iMax = v.length();
		byte[] temp = new byte[iMax];
		int t = 0;
		for (int i = 0; i < iMax;) {
			char ch = v.charAt(i++);
			switch (ch) {
			case '+':
				temp[t++] = SP;
				break;

			case '%':
				String s = v.substring(i, i + 2);
				i += 2;

				try {
					temp[t++] = (byte) Integer.valueOf(s, 16).intValue();
				} catch (NumberFormatException e) {
					return null;
				}
				break;

			default:
				temp[t++] = (byte) ch;
				break;

			}
		}

		byte[] r = new byte[t];
		System.arraycopy(temp, 0, r, 0, t);
		return r;
	}

	/**
	 * Encodes a string.
	 * 
	 * @param v
	 *            plain-text value
	 */
	public String encode(String v) {
		return standardMap.encode(v);
	}

	/**
	 * Encodes a byte array.
	 */
	public String encode(byte[] v) {
		return standardMap.encode(v);
	}

	/**
	 * Encodes every character in a string.
	 */
	public String encodeFully(String v) {
		return fullyMap.encode(v);
	}

	/**
	 * Encodes a byte array.
	 */
	public String encodeFully(byte[] v) {
		return fullyMap.encode(v);
	}

	private Map standardMap = new StandardMap();
	private Map fullyMap = new FullyMap();
}
