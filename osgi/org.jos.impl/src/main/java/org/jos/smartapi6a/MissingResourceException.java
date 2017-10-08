package org.jos.smartapi6a;

/**
 * This exception is thrown by Request.getObject() when a resource is missing,
 * instead of returning null.
 */
public final class MissingResourceException extends Exception {
	private static final long serialVersionUID = 0L;

	/**
	 * Creates an exception.
	 */
	public MissingResourceException(Request r) {
		super("'" + r.getSpec() + "' resource is missing");
	}
}
