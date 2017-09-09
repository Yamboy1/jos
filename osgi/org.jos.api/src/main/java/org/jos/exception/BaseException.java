package org.jos.exception;

/**
 * A base exception with the ability to set a message.
 */
public class BaseException extends Exception {
	private static final long serialVersionUID = 0L;

	private String message = null;

	/**
	 * Creates an exception.
	 */
	public BaseException() {
		super();
	}

	/**
	 * Creates an exception, with a given message.
	 */
	public BaseException(String v) {
		super();
		setMessage(v);
	}

	/**
	 * Gets Message property.
	 */
	public String getMessage() {
		return message;
	}

	/**
	 * Sets Message property.
	 */
	public void setMessage(String v) {
		message = v;
	}
}
