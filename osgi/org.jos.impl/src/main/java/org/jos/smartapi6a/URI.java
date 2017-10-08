package org.jos.smartapi6a;

import org.jos.smartapi6a.impl.BasicRequest;
import org.jos.smartapi6a.impl.BootFactory;

/**
 * This class implements the Request interface and maintains the default
 * plug-in. This is a single request for an object. An application program makes
 * a request to the Smart API for an object. An object is returned, or an
 * exception is thrown. The default plug-in is
 * gchii.library.smartplugin1c.SmartPluginFactory. The default plug-in
 * identifies itself as an instance of org.jos.smartapi5a.Factory:
 * META-INF/service/org.jos.smartapi5a.Factory.
 */
public class URI extends BasicRequest implements Request {
	private static final String VERSION = "1.0-33";

	/**
	 * Gets Factory property.
	 */
	public static Controller getFactory() {
		if (factory == null) {
			installFactory();
		}
		return factory;
	}

	/**
	 * Sets Factory property. The default factory is the smart object factory
	 * (ObjectFactory) class implemented in the org.jos.gchii.factory1c package.
	 * Use this method to override the default controller for all subsequent
	 * instances of URI.
	 */
	public static synchronized void setFactory(Controller v) {
		if (factory == null) {
			factory = v;
		}
	}

	/**
	 * Installs a factory.
	 */
	private static synchronized void installFactory() {
		if (factory == null && !initFlag) {
			initFlag = true;

			try {
				String spec = "object:class:gchii.library.smartplugin1e.SmartPluginFactory?instanceof=org.jos.smartapi6a.Controller";
				Request r = new BasicRequest(new BootFactory(), spec);
				try {
					factory = (Controller) r.getObject();
					println("Installed default factory (1g).");
				} catch (Throwable e) {
					System.out.println("[" + e.getClass().getName() + "] " + e.getMessage());
				}
				return;
			} catch (Throwable ignore) {
			}
		}
	}

	private static void println(String m) {
		System.out.println("Smart API (6a) " + VERSION + ": " + m);
	}

	private static Controller factory;

	private static boolean initFlag;

	/**
	 * Creates a URI, with a given value.
	 */
	public URI(String v) {
		super(getFactory(), v);
	}
}
