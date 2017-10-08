package org.jos.smartapi6a;

import java.util.Enumeration;

/**
 * A container for plugins. This is an interface for all smart object factories.
 */
public interface Factory extends Controller {
	/**
	 * Adds a controller.
	 */
	public abstract void addController(Controller v);

	/**
	 * Adds a controller, by class name.
	 *
	 * @param cn
	 *            a fully-qualified class name
	 */
	public abstract void addControllerFor(String cn);

	/**
	 * Adds a controller descriptor, by scheme and class name.
	 *
	 * @param s
	 *            a scheme
	 * @param cn
	 *            a fully-qualified class name
	 */
	public abstract void addController(String s, String cn);

	/**
	 * Adds a local controller, by class name.
	 *
	 * @param cn
	 *            a relative class name.
	 */
	public abstract void addLocalController(String cn);

	/**
	 * Adds a local controller descriptor, by scheme and class name.
	 *
	 * @param s
	 *            a scheme
	 * @param cn
	 *            a relative class name.
	 */
	public abstract void addLocalController(String s, String cn);

	/**
	 * Removes a controller.
	 */
	public abstract void removeController(Controller v);

	/**
	 * Removes a controller by name.
	 */
	public abstract void removeControllerFor(String v);

	/**
	 * Gets a copy of the controller list.
	 */
	public abstract Controller[] getControllerList();

	/**
	 * Gets Schemes property.
	 *
	 * @return an enumeration of scheme names (java.lang.String)
	 */
	public abstract Enumeration<String> getSchemes();
}
