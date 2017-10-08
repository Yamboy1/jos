package org.jos.smartapi6a.impl;

import org.jos.smartapi6a.Request;
import org.jos.smartapi6a.Scheme;
import org.jos.smartapi6a.Suffix;

class BasicSuffix implements Suffix {
	/**
	 * Creates a suffix.
	 */
	BasicSuffix(Request r, Scheme s) {
		request = r;
		scheme = s;
	}

	/**
	 * Gets Request property.
	 */
	public Request getRequest() {
		return request;
	}

	/**
	 * Gets Scheme property.
	 */
	public Scheme getScheme() {
		return scheme;
	}

	/**
	 * Gets Path property.
	 */
	public String getPath() {
		return path;
	}

	/**
	 * Gets QueryString property.
	 */
	public String getQueryString() {
		return request.getQueryString();
	}

	/**
	 * Gets Fragment property.
	 */
	public String getFragment() {
		return request.getFragment();
	}

	/**
	 * Gets Parameter[] property.
	 */
	public String getParameter(String n) {
		return request.getParameter(n);
	}

	private Request request;
	private Scheme scheme;
	private String path;
}
