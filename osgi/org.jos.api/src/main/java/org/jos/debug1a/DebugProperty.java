package org.jos.debug1a;

/**
 * A controller should implement this interface to enable the debug property.
 */
public interface DebugProperty {
	/**
	 * Gets Debug property.
	 */
	public abstract boolean isDebug();

	/**
	 * Sets Debug property.
	 */
	public abstract void setDebug(boolean flag);

	/**
	 * Prints a message.
	 */
	public abstract void debug(String message);
}
