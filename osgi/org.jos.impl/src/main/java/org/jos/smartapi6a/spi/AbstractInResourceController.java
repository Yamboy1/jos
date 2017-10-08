package org.jos.smartapi6a.spi;

import java.io.InputStream;

import org.jos.smartapi6a.Request;

/**
 * This is a base class for in:resource: controllers.
 */
public abstract class AbstractInResourceController extends BaseController {
	private final String sn;
	private String basepath;

	/**
	 * Creates a controller.
	 * 
	 * @param v
	 *            path-to-local-resource(s)
	 */
	protected AbstractInResourceController(String path) {
		super("in:resource:");
		sn = getClass().getName();
		debug("[" + sn + " constructor] scheme=" + getScheme().getText() + " path=" + path);
		basepath = path;
		debug("basepath=" + basepath);
	}

	/**
	 * Gets Match property.
	 */
	@Override
	public final boolean isMatch(Request r) {
		if (!super.isMatch(r)) {
			return false;
		}

		return r.getPath().startsWith(basepath);
	}

	/**
	 * Gets Object property.
	 */
	@Override
	protected final Object findObject(Request r) throws Exception {
		debug("[" + sn + " findObject]  r=" + r);
		String name = "/" + r.getPath();
		debug("[" + sn + " findObject] name=" + name);
		InputStream result = null;
		try {
			result = getClass().getResourceAsStream(name);
		} catch (Throwable e) {
			//
			// continue; allow other implementations of the in:resource: (was
			// resource:) scheme
			debug("[" + sn + " findObject] allow other implementations of the in:resource: scheme.");
			return null;
		}
		debug("  result=" + result);
		return result;
	}
}
