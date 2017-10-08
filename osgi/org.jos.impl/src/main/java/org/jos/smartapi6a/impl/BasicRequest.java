package org.jos.smartapi6a.impl;

import org.jos.program2a.ParameterList;
import org.jos.smartapi6a.Controller;
import org.jos.smartapi6a.MissingResourceException;
import org.jos.smartapi6a.Request;
import org.jos.smartapi6a.Scheme;

public class BasicRequest implements Request {
	/**
	 * Creates a request.
	 */
	public BasicRequest(Controller c, String s) {
		this(c, s, null);
	}

	/**
	 * Creates a request.
	 */
	public BasicRequest(Controller c, String s, Scheme sc) {
		trace("constructor( c, s=" + s + ", sc=" + sc);

		controller = c;
		spec = s;

		{
			//
			// drop fragment
			//
			String match = "#";
			int pos = s.indexOf(match);
			if (pos < 0) {
				// no fragment
			} else {
				fragment = s.substring(pos + match.length());
				s = s.substring(0, pos);
			}
		}

		{
			//
			// drop queryString
			//
			String match = "?";
			int pos = s.indexOf(match);
			if (pos < 0) {
				// no queryString
			} else {
				queryString = s.substring(pos + match.length());
				s = s.substring(0, pos);
			}
		}

		if (sc == null) {
			{
				//
				// drop path
				//
				String match = "/";
				int pos = s.indexOf(match);
				if (pos < 0) {
					// no path
				} else {
					path = s.substring(pos + match.length());
					s = s.substring(0, pos);
				}
			}

			scheme = new BasicScheme(s);
			return;
		}

		debug("s=" + s);
		debug("s.length=" + s.length());

		debug("sc.text=" + sc.getText());

		scheme = sc;
		int len = scheme.getText().length();
		debug("sc.text.len=" + len);
		if (s.length() > len) {
			path = s.substring(len);
		}
	}

	/**
	 * Gets Parameter[] property.
	 */
	public String getParameter(String n) {
		if (parameterList == null) {
			if (queryString == null) {
				return null;
			}

			BasicParameterList list = new BasicParameterList();
			list.parseArguments(queryString);
			parameterList = list;
		}

		return parameterList.getValue(n);
	}

	/**
	 * Gets Match property.
	 */
	public boolean isMatch() {
		return controller.isMatch(this);
	}

	/**
	 * Gets Object property.
	 */
	public Object getObject() throws Exception {
		trace("request=" + this + " getObject()");

		Object o = controller.getObject(this);
		if (o == null) {
			throw new MissingResourceException(this);
		}
		return o;
	}

	/**
	 * Gets Request property.
	 */
	public Request getRequest(String spec) {
		return new BasicRequest(controller, spec);
	}

	/**
	 * Gets Request property.
	 */
	public Request getRequest(Scheme v) {
		return new BasicRequest(controller, spec, v);
	}

	/**
	 * Gets request from suffix.
	 */
	public Request getRequestFromSuffix() {
		return getRequestFromSuffix(new BasicScheme(""));
	}

	/**
	 * Gets request from suffix.
	 */
	public Request getRequestFromSuffix(String sc) {
		return getRequestFromSuffix(new BasicScheme(sc));
	}

	/**
	 * Gets request from suffix.
	 */
	public Request getRequestFromSuffix(Scheme sc) {
		String newSpec = sc.getText();

		{
			String s = getPath();
			if (s != null) {
				newSpec += s;
			}
		}

		{
			String s = getQueryString();
			if (s != null) {
				newSpec += "?" + s;
			}
		}

		{
			String s = getFragment();
			if (s != null) {
				newSpec += "#" + s;
			}
		}

		return new BasicRequest(controller, newSpec);
	}

	/**
	 * Gets Controller property.
	 */
	public Controller getController() {
		return controller;
	}

	/**
	 * Sets Controller property.
	 */
	public void setController(Controller v) {
		controller = v;
	}

	private Controller controller;

	/**
	 * Gets Spec property.
	 */
	public String getSpec() {
		return spec;
	}

	private String spec;

	/**
	 * Gets Scheme property.
	 */
	public Scheme getScheme() {
		return scheme;
	}

	private Scheme scheme;

	/**
	 * Gets Path property.
	 */
	public String getPath() {
		return path;
	}

	private String path;

	/**
	 * Gets QueryString property.
	 */
	public String getQueryString() {
		return queryString;
	}

	private String queryString;

	/**
	 * Gets Fragment property.
	 */
	public String getFragment() {
		return fragment;
	}

	private String fragment;

	/**
	 * Converts to string.
	 */
	public String toString() {
		StringBuffer buf = new StringBuffer();
		buf.append("request[");
		if (getSpec() != null) {
			buf.append(" spec=").append(getSpec());
		}
		if (getScheme() != null) {
			buf.append(" scheme=").append(getScheme().getText());
		}
		if (getPath() != null) {
			buf.append(" path=").append(getPath());
		}
		if (getQueryString() != null) {
			buf.append(" queryString=").append(getQueryString());
		}
		if (getFragment() != null) {
			buf.append(" fragment=").append(getFragment());
		}
		buf.append(" ]");
		return buf.toString();
	}

	/**
	 * Displays a message.
	 */
	protected void info(String v) {
		System.out.println("[BasicRequest] INFO: " + v);
	}

	/**
	 * Displays a message.
	 */
	protected void trace(String v) {
	}

	/**
	 * Displays a message.
	 */
	protected void debug(String v) {
	}

	private ParameterList parameterList;
}
