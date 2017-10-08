package org.jos.smartapi6a;

public interface Suffix {
	/**
	 * Gets Request property.
	 */
	public abstract Request getRequest();

	/**
	 * Gets Scheme property.
	 */
	public abstract Scheme getScheme();

	/**
	 * Gets Path property.
	 */
	public abstract String getPath();

	/**
	 * Gets QueryString property.
	 */
	public abstract String getQueryString();

	/**
	 * Gets Fragment property.
	 */
	public abstract String getFragment();

	/**
	 * Gets Parameter[] property.
	 */
	public abstract String getParameter(String n);
}
