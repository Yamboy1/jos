package org.jos.smartapi6a.spi;

import java.util.Enumeration;
import java.util.NoSuchElementException;

/**
 * Enables a controller to enumerate schemes.
 */
public class BasicSchemeEnumeration implements Enumeration<String> {
	private static final int STEP = 10;

	/**
	 * Creates an enumeration.
	 */
	public BasicSchemeEnumeration() {
	}

	/**
	 * Creates an enumeration.
	 */
	public BasicSchemeEnumeration(String v) {
		if (v == null) {
			return;
		}

		payload = new String[1];
		payload[0] = v;
	}

	/**
	 * Creates an enumeration.
	 */
	public BasicSchemeEnumeration(String[] v) {
		if (v == null) {
			return;
		}

		int iMax = v.length;
		payload = new String[iMax];
		System.arraycopy(v, 0, payload, 0, iMax);
	}

	/**
	 * Add schemes.
	 */
	public void addSchemes(Enumeration<String> v) {
		if (v == null) {
			return;
		}

		String[] list = new String[STEP];
		int count = 0;
		for (;;) {
			if (!v.hasMoreElements()) {
				break;
			}

			String item = (String) v.nextElement();
			if (item == null) {
				continue;
			}

			try {
				list[count] = item;
				count++;
			} catch (ArrayIndexOutOfBoundsException e) {
				int iMax = list.length;
				String[] temp = new String[iMax + STEP];
				System.arraycopy(list, 0, temp, 0, iMax);
				list = temp;

				list[count++] = item;
				count++;
			}
		}

		addSchemes(list, count);
	}

	protected void addSchemes(String[] v, int c) {
		if (v == null) {
			return;
		}

		if (c == 0) {
			return;
		}

		if (payload == null) {
			payload = new String[c];
			System.arraycopy(v, 0, payload, 0, c);
			return;
		}

		String[] temp = new String[payload.length + c];

		System.arraycopy(payload, 0, temp, 0, payload.length);
		System.arraycopy(v, 0, temp, payload.length, c);
		payload = temp;
	}

	/**
	 * Gets MoreElements property.
	 */
	public boolean hasMoreElements() {
		// FIXED: Found by SchemeEnumeration_Test
		if (payload == null) {
			return false;
		}

		return (index < payload.length);
	}

	/**
	 * Gets NextElement property.
	 */
	public String nextElement() {
		String r;

		try {
			r = payload[index];
		} catch (NullPointerException e) {
			throw new NoSuchElementException();
		} catch (ArrayIndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}

		index++;
		return r;
	}

	private String[] payload;
	private int index;
}
