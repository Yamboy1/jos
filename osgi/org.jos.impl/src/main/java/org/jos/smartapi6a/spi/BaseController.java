package org.jos.smartapi6a.spi;

import java.util.Enumeration;

import org.jos.smartapi6a.Request;
import org.jos.smartapi6a.Scheme;
import org.jos.smartapi6a.impl.BasicScheme;

/**
 * Typically, a plug-in extends this class. This is an excellent base class for
 * a plug-in.
 */
public abstract class BaseController extends BaseParentController {
	/**
	 * Creates a controller.
	 */
	public BaseController(String v) {
		scheme = new BasicScheme(v);
	}

	/**
	 * Gets Schemes property.
	 */
	public Enumeration<String> getSchemes() {
		return new BasicSchemeEnumeration(scheme.getText());
	}

	/**
	 * Returns true if the uniform resource identifier matches the scheme for
	 * this controller.
	 *
	 * @param v
	 *            a uniform resource identifier.
	 */
	public boolean isMatch(Request r) {
		return getScheme().isMatch(r);
	}

	/**
	 * Gets Object[] property.
	 */
	public final Object getObject(Request request) throws Exception {
		if (!isMatch(request)) {
			return null;
		}

		trace("getObject( request=" + request + " )");

		Request forward = request.getRequest(getScheme());

		return findObject(forward);
	}

	/**
	 * This method is implemented by a service provider.
	 */
	protected abstract Object findObject(Request r) throws Exception;

	/**
	 * Debug event.
	 */
	protected void onDebug() {
		debug(getScheme().getText() + " controller.debug=" + isDebug());
	}

	/**
	 * Gets Scheme property.
	 */
	protected Scheme getScheme() {
		return scheme;
	}

	/**
	 * Gets ProgramException property.
	 */
	protected Exception WARN(String v) {
		return new Exception(v);
	}

	/**
	 * Gets ParameterException[] property.
	 */
	protected Exception BAD_PARAM(String v) {
		return new Exception(v + " parameter is missing.");
	}

	/**
	 * Gets BAD_RESOURCE[] property.
	 */
	protected Exception BAD_RESOURCE(String v) {
		return new Exception("'" + v + "' resource is missing.");
	}

	private Scheme scheme;
}
