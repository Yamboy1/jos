package org.jos.smartapi6a;

/**
 * A controller descriptor supports on-demand loading of controllers by class
 * name.
 */
public interface ControllerDescriptor extends Controller {
	/**
	 * Gets Active property.
	 *
	 * @return When subcontroller has been installed, returns true.
	 */
	public abstract boolean isActive();

	/**
	 * Gets SubcontrollerClassname property.
	 *
	 * @return fully-qualified class name of subcontroller.
	 */
	public abstract String getSubcontrollerClassname();

	/**
	 * Gets Subcontroller property. Only one attempt is made to load a
	 * subcontroller by name.
	 *
	 * @return a controller, or null.
	 */
	public abstract Controller getSubcontroller();
}
