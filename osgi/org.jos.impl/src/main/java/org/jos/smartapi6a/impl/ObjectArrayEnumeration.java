package org.jos.smartapi6a.impl;

import java.util.Enumeration;
import java.util.NoSuchElementException;

/**
 * A basic enumeration for any object array.
 */
final class ObjectArrayEnumeration implements Enumeration<Object> {
	/**
	 * Creates an enumeration.
	 */
	ObjectArrayEnumeration(Object[] v) {
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
	public Object nextElement() {
		if (!hasMoreElements()) {
			throw new NoSuchElementException("" + index);
		}

		return list[index++];
	}

	private Object[] list;
	private int index;
}
