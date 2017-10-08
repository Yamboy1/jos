package org.jos.smartapi6a.impl;

import org.jos.smartapi6a.Request;
import org.jos.smartapi6a.Scheme;
import org.jos.smartapi6a.Suffix;

/**
 * Enables a controller to match a location to a scheme.
 */
public class BasicScheme implements Scheme {
	/**
	 * Creates a scheme.
	 */
	public BasicScheme(String v) {
		setText(v);
	}

	/**
	 * Gets Text property.
	 */
	public String getText() {
		return text;
	}

	/**
	 * Sets Text property.
	 */
	protected void setText(String v) {
		text = v;

		if (text != null) {
			match = text.toLowerCase();
		}
	}

	/**
	 * Gets Match[] property.
	 */
	public boolean isMatch(Request r) {
		if (text == null || r == null) {
			return false;
		}

		String v = r.getScheme().getText();
		if (v.length() < match.length()) {
			return false;
		}

		String s;
		try {
			s = v.substring(0, match.length()).toLowerCase();
		} catch (StringIndexOutOfBoundsException e) {
			return false;
		} catch (Throwable e) {
			return false;
		}

		return match.equals(s);
	}

	/**
	 * Gets Prefix[] property.
	 */
	public String getPrefix(Request r) {
		if (!isMatch(r)) {
			return null;
		}

		return r.getScheme().getText().substring(0, text.length());
	}

	/**
	 * Gets Suffix[] property.
	 */
	public Suffix getSuffix(Request r) {
		if (!isMatch(r)) {
			return null;
		}

		return new BasicSuffix(r, this);
	}

	private String text;
	private String match;
}
