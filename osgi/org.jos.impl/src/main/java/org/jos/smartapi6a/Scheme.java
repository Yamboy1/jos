package org.jos.smartapi6a;

/**
 * Enables a controller to match a location to a scheme.
 */
public interface Scheme {
	/**
	 * Gets Text property.
	 *
	 * @return An original scheme, typically lowercase.
	 */
	public abstract String getText();

	/**
	 * Gets Match[] property.
	 *
	 * @param v
	 *            a location
	 * @return Returns true when location starts with scheme.
	 */
	public abstract boolean isMatch(Request r);

	/**
	 * Gets Prefix[] property.
	 *
	 * @param v
	 *            a location
	 * @return When matching, returns first part of location. Otherwise, null.
	 */
	public abstract String getPrefix(Request v);

	/**
	 * Gets Suffix[] property.
	 *
	 * @param v
	 *            a location
	 * @return When matching, returns last part of location. Otherwise, null.
	 */
	public abstract Suffix getSuffix(Request r);
}
