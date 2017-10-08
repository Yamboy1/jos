package org.jos.smartapi6a;

import java.util.Enumeration;

import org.jos.debug1a.DebugProperty;

/**
 * An interface for a plugin. A controller is the core interface of a Smart API.
 * A controller is an generic interface to very specific object factories.
 */
@SuppressWarnings("deprecation")
public interface Controller extends DebugProperty {
	/**
	 * Determines if a controller accepts a given URI.
	 *
	 * @param r
	 *            a request
	 */
	public abstract boolean isMatch(Request r);

	/**
	 * Gets Object[] property. Implementations must return an object as
	 * identified by a given URI, or throw no exceptions and return null.
	 *
	 * @param r
	 *            a request
	 */
	public abstract Object getObject(Request r) throws Exception;

	/**
	 * Gets Schemes property.
	 *
	 * @return Returns an instance of Enumeration, or null.
	 */
	public abstract Enumeration<String> getSchemes();
}
