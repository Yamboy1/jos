package org.jos.smartapi6a.spi;

import java.util.Enumeration;

import org.jos.debug1a.DebugProperty;
import org.jos.smartapi6a.Controller;
import org.jos.smartapi6a.Factory;
import org.jos.smartapi6a.Request;

/**
 * This is an excellent base class for a plug-in container.
 */
@SuppressWarnings("deprecation")
public abstract class BaseFactory extends BaseParentController implements Factory {
	private static final int SIZE_MIN = 10;

	/**
	 * Creates a smart factory.
	 */
	protected BaseFactory() {
		data = new Controller[SIZE_MIN];
	}

	/**
	 * Determines if any controller can accept a given URI.
	 *
	 * @param v
	 *            a URI
	 * @return When controller accepts URI, true. Otherwise, false.
	 */
	public boolean isMatch(Request r) {
		trace("isMatch( r=" + r + " )");

		if (r == null) {
			return false;
		}

		Controller[] list = getControllerList();
		int iMax = list.length;
		for (int i = 0; i < iMax; i++) {
			if (list[i].isMatch(r)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Gets Object[] property.
	 *
	 * @param r
	 *            a request
	 * @return an object, or null
	 */
	public Object getObject(Request r) throws Exception {
		trace("getObject( request=" + r + " )");

		if (r == null) {
			return null;
		}

		Controller[] list = getControllerList();
		int iMax = list.length;
		for (int i = 0; i < iMax; i++) {
			debug("[" + i + "] " + list[i].getClass().getName());
			if (list[i].isMatch(r)) {
				Object o = list[i].getObject(r);
				if (o != null) {
					return o;
				}
			}
		}
		return null;
	}

	/**
	 * Adds a controller.
	 *
	 * @param v
	 *            a controller
	 */
	public synchronized void addController(Controller v) {
		if (v == null) {
			return;
		}

		if (isController(v)) {
			return;
		}

		int iMax = data.length;
		for (int i = 0; i < iMax; i++) {
			if (data[i] == null) {
				data[i] = v;
				return;
			}
		}

		Controller[] temp = new Controller[iMax + SIZE_MIN];
		System.arraycopy(data, 0, temp, 0, iMax);
		data = temp;
		data[iMax] = v;
	}

	/**
	 * Adds a controller, by class name.
	 *
	 * @param v
	 *            name of a controller
	 */
	public void addControllerFor(String v) {
		trace("addControllerFor( v=" + v + " )");

		if (v == null) {
			return;
		}

		debug("addControllerFor( v='" + v + "' )");

		Controller c;
		try {
			c = (Controller) Class.forName(v).newInstance();
		} catch (InstantiationException e) {
			warn("Could not load '" + v + "' controller; [" + e.getClass().getName() + "] " + e.getMessage());
			return;
		} catch (Throwable e) {
			debug("Could not load '" + v + "' controller; [" + e.getClass().getName() + "] " + e.getMessage());
			return;
		}

		debug("Adding '" + v + "' controller.");

		addController(c);
	}

	/**
	 * Removes a controller.
	 */
	public synchronized void removeController(Controller v) {
		if (v == null) {
			return;
		}

		int iMax = data.length;
		for (int i = 0; i < iMax; i++) {
			if (data[i] == v) {
				data[i] = null;
			}
		}
	}

	/**
	 * Removes a controller by name.
	 *
	 * @param v
	 *            name of controller
	 */
	public void removeControllerFor(String v) {
		if (v == null) {
			return;
		}

		Controller[] list = getControllerList();
		int iMax = list.length;
		for (int i = 0; i < iMax; i++) {
			String cn = list[i].getClass().getName();
			if (cn.equals(v)) {
				removeController(list[i]);
			}
		}
	}

	/**
	 * Gets Controller[] property.
	 *
	 * @param v
	 *            a controller
	 * @return when exact match, returns true. Otherwise, returns false.
	 */
	protected boolean isController(Controller v) {
		int iMax = data.length;
		for (int i = 0; i < iMax; i++) {
			if (data[i] == v) {
				return true;
			}
		}

		return false;
	}

	/**
	 * Gets a copy of the controller list.
	 *
	 * @return array of controllers
	 */
	public synchronized Controller[] getControllerList() {
		int iMax = data.length;
		Controller[] list = new Controller[iMax];
		int len = 0;
		for (int i = 0; i < iMax; i++) {
			if (data[i] == null) {
				continue;
			}

			list[len++] = data[i];
		}
		Controller[] temp = new Controller[len];
		System.arraycopy(list, 0, temp, 0, len);
		return temp;
	}

	/**
	 * Gets Schemes property.
	 * 
	 * @return an enumeration of scheme names (java.lang.String)
	 */
	public Enumeration<String> getSchemes() {
		Controller[] list = getControllerList();
		BasicSchemeEnumeration r = new BasicSchemeEnumeration();
		int iMax = list.length;
		for (int i = 0; i < iMax; i++) {
			r.addSchemes(list[i].getSchemes());
		}
		return r;
	}

	/**
	 * Debug event.
	 */
	protected void onDebug() {
		boolean v = isDebug();

		Controller[] list = getControllerList();
		int iMax = list.length;
		for (int i = 0; i < iMax; i++) {
			Controller item = list[i];

			if (item instanceof DebugProperty) {
				((DebugProperty) item).setDebug(v);
			}
		}
	}

	/**
	 * Adds a controller from this package.
	 *
	 * @param v
	 *            name of local controller
	 */
	public void addLocalController(String v) {
		String pn = getPackageName();
		if (pn.length() > 0) {
			pn += ".";
		}
		addControllerFor(pn + v);
	}

	/**
	 * Adds a controller descriptor.
	 *
	 * @param s
	 *            a scheme
	 * @param cn
	 *            name of controller
	 */
	public void addController(String s, String cn) {
		addController(new BasicControllerDescriptor(s, cn));
	}

	/**
	 * Adds a local controller descriptor.
	 *
	 * @param s
	 *            a scheme
	 * @param cn
	 *            name of local controller
	 */
	public void addLocalController(String s, String cn) {
		String pn = getPackageName();
		if (pn.length() > 0) {
			pn += ".";
		}
		addController(new BasicControllerDescriptor(s, pn + cn));
	}

	/**
	 * Adds controller descriptor.
	 *
	 * @param line
	 *            a line
	 */
	protected void addControllerDescriptor(String line) {
		trace("addControllerDescriptor( line=" + line + " )");

		String match = "=";
		int pos = line.indexOf(match);
		if (pos < 0) {
			addControllerFor(line);
		} else {
			String classname = line.substring(0, pos);
			String scheme = line.substring(pos + match.length());
			addControllerDescriptor(scheme, classname);
		}
	}

	/**
	 * Adds a controller descriptor.
	 *
	 * @param sc
	 *            a scheme
	 * @param cn
	 *            a classname
	 */
	protected void addControllerDescriptor(String sc, String cn) {
		trace("addControllerDescriptor( sc=" + sc + ", cn=" + cn + " )");

		addController(new BasicControllerDescriptor(sc, cn));
	}

	/**
	 * Gets PackageName property.
	 *
	 * @return a package name
	 */
	protected String getPackageName() {
		return cn2pn(getClass().getName());
	}

	/**
	 * Converts from class name (cn) to package name (pn).
	 *
	 * @param v
	 *            a class name
	 * @return a package name
	 */
	protected String cn2pn(String v) {
		if (v == null) {
			return null;
		}

		int pos = v.lastIndexOf(".");
		return (pos < 0 ? "" : v.substring(0, pos));
	}

	private Controller[] data;
}
