package org.jos.uritool;

/**
 * This map provides standard URI encoding.
 */
final class StandardMap extends BaseMap {
	/**
	 * Creates a map.
	 */
	public StandardMap() {
		String UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		String LOWERCASE = UPPERCASE.toLowerCase();
		String DIGITS = "0123456789";
		String PLAINTEXT = UPPERCASE + LOWERCASE + DIGITS;

		int iMax = PLAINTEXT.length();
		for (int i = 0; i < iMax; i++) {
			setEncoding(PLAINTEXT.charAt(i), PLAINTEXT.substring(i, i + 1));
		}
	}
}
