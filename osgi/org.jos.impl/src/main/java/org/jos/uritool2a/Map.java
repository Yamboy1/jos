package org.jos.uritool2a;

/**
 * This is an interface for a character encoder.
 */
public interface Map {
	/**
	 * Encodes a character.
	 * 
	 * @param v
	 *            a character
	 */
	public abstract String encode(char v);

	/**
	 * Encodes a string.
	 */
	public abstract String encode(String v);

	/**
	 * Encodes a byte array.
	 */
	public abstract String encode(byte[] v);
}
