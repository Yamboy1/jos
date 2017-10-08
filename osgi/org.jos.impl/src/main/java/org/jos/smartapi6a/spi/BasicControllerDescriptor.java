package org.jos.smartapi6a.spi;

import org.jos.smartapi6a.Controller;
import org.jos.smartapi6a.ControllerDescriptor;
import org.jos.smartapi6a.Request;

/**
 * A controller descriptor supports on-demand loading of controllers by class
 * name.
 */
public class BasicControllerDescriptor extends BaseController implements ControllerDescriptor {
	/**
	 * Creates a controller.
	 *
	 * @param v
	 *            scheme
	 * @param cn
	 *            class name for subcontroller
	 */
	public BasicControllerDescriptor(String s, String cn) {
		super(s);
		subcontrollerClassname = cn;
	}

	/**
	 * Gets Active property.
	 *
	 * @return When subcontroller has been installed, returns true.
	 */
	public boolean isActive() {
		return (subcontroller != null);
	}

	/**
	 * Gets SubcontrollerClassname property.
	 *
	 * @return fully-qualified class name of subcontroller.
	 */
	public String getSubcontrollerClassname() {
		return subcontrollerClassname;
	}

	/**
	 * Gets Subcontroller property. Only one attempt is made to load a
	 * subcontroller by name.
	 *
	 * @return a controller, or null.
	 */
	public Controller getSubcontroller() {
		if (!isInit) {
			trace("getSubcontroller()");

			isInit = true;
			try {
				Object o = Class.forName(subcontrollerClassname).newInstance();
				if (o instanceof Controller) {
					debug("subcontroller.class=(5b)");
					subcontroller = (Controller) o;
				}
				// 20171007 gchii
				/*
				if (o instanceof org.jos.smartapi4a.Controller) {
					debug("subcontroller.class=(4a)");
					subcontroller = new Controller6aAdapter4a((org.jos.smartapi4a.Controller) o);
				}
				*/
				if (subcontroller != null) {
					info("'" + subcontrollerClassname + "' controller added for '" + getScheme().getText()
							+ "' scheme.");
					onDebug();
				}
			} catch (Throwable e) {
				debug("[" + e.getClass().getName() + "] " + e.getMessage());
			}
		}

		return subcontroller;
	}

	/**
	 * Gets Object[] property.
	 *
	 * @param a
	 *            resource identifier.
	 * @return an object, or null.
	 */
	protected Object findObject(Request r) throws Exception {
		try {
			return getSubcontroller().getObject(r);
		} catch (NullPointerException e) {
			return null;
		}
	}

	/**
	 * Debug event.
	 */
	@SuppressWarnings("deprecation")
	protected void onDebug() {
		if (subcontroller == null) {
			return;
		}

		try {
			subcontroller.setDebug(isDebug());
		} catch (ClassCastException ignore) {
		} catch (NullPointerException ignore) {
		}
	}

	/**
	 * Converts to string.
	 */
	public String toString() {
		return "<descriptor subcontroller=\"" + subcontrollerClassname + "\">";
	}

	private String subcontrollerClassname;
	private boolean isInit;
	private Controller subcontroller;
}
