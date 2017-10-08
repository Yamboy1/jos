package org.jos.smartapi6a;

/**
 * A single request for an object.
 */
public interface Request {
	/**
	 * Gets Controller property.
	 */
	public abstract Controller getController();

	/**
	 * Gets Spec property.
	 */
	public abstract String getSpec();

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
	 * Gets Object property.
	 */
	public abstract Object getObject() throws Exception;

	/**
	 * Gets Parameter[] property.
	 */
	public abstract String getParameter(String n);

	/**
	 * Gets Request[] property.
	 */
	public abstract Request getRequest(Scheme s);

	/**
	 * Gets Request[] property.
	 */
	public abstract Request getRequest(String spec);

	/**
	 * Gets request from suffix.
	 */
	public abstract Request getRequestFromSuffix();

	/**
	 * Gets request from suffix.
	 *
	 * @param sc
	 *            new scheme
	 */
	public abstract Request getRequestFromSuffix(Scheme sc);

	/**
	 * Gets request from suffix.
	 *
	 * @param sc
	 *            new scheme
	 */
	public abstract Request getRequestFromSuffix(String sc);

	/**
	 * Gets Match property.
	 *
	 * @return Returns true when specification has a corresponding factory.
	 */
	public abstract boolean isMatch();
}
