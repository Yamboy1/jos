package org.jos.uritool;

/**
 * This is an optional base class for a map.
 */
abstract class BaseMap implements Map {
	private static final int STEP = 0x100;

	/**
	 * Creates a map.
	 */
	protected BaseMap() {
	}

	/**
	 * Encodes a character.
	 */
	public String encode(char c) {
		int index = c & 0xFF;
		String item = list[index];
		if (item == null) {
			item = getEncoding(c);
			setEncoding(c, item);
		}
		return item;
	}

	/**
	 * Encodes a string.
	 */
	public String encode(String v) {
		if (v == null) {
			return null;
		}

		StringBuffer buffer = new StringBuffer();

		int iMax = v.length();
		for (int i = 0; i < iMax; i++) {
			buffer.append(encode(v.charAt(i)));
		}

		return buffer.toString();
	}

	/**
	 * Encodes a byte array.
	 */
	public String encode(byte[] v) {
		if (v == null) {
			return null;
		}

		StringBuffer buffer = new StringBuffer();

		int iMax = v.length;
		for (int i = 0; i < iMax; i++) {
			buffer.append(encode((char) (v[i] & 0xFF)));
		}

		return buffer.toString();
	}

	/**
	 * Sets an encoding.
	 */
	protected void setEncoding(char c, String v) {
		int index = c & 0xFF;
		list[index] = v;
	}

	/**
	 * Encodes a character.
	 */
	protected String getEncoding(char c) {
		if (c == ' ') {
			return "+";
		}

		String s = Integer.toHexString((int) c).toUpperCase();
		switch (s.length()) {
		case 1:
			return "%0" + s;

		case 2:
			return "%" + s;

		default:
			return s;

		}
	}

	private String[] list = new String[STEP];
}
