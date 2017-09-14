package org.jos.program2c;

public interface Environment {
	/**
	 * Gets Attribute[] property.
	 * @param key
	 * @return
	 */
	public abstract String getAttribute(String key);

	/**
	 * Sets Attribute[] property.
	 * @param key
	 * @param value
	 */
	public void setAttribute(String key, Object value);
}
