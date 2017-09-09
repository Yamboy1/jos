package org.jos.program.impl;

import java.util.Enumeration;
import java.util.NoSuchElementException;

/**
 * A basic enumeration for any object array.
 */
final class ObjectArrayEnumeration implements Enumeration<String> {
	private String[] list;
	private int index;

	/**
	 * Creates an enumeration.
	 */
	public ObjectArrayEnumeration(final String[] v) {
		index = 0;
		list = v;
	}

	/**
	 * Determines if this enumeration has more elements.
	 */
	public boolean hasMoreElements() {
		for (;;) {
			try {
				if (list[index] == null) {
					index++;
					continue;
				}
			} catch (NullPointerException e) {
				return false;
			} catch (ArrayIndexOutOfBoundsException e) {
				return false;
			}

			return true;
		}
	}

	/**
	 * Returns the next element.
	 */
	public String nextElement() {
		if (!hasMoreElements()) {
			throw new NoSuchElementException("" + index);
		}

		return list[index++];
	}
}
