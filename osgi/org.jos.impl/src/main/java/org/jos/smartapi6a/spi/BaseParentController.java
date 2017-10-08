package org.jos.smartapi6a.spi;

import org.jos.smartapi6a.Controller;

/**
 * A parent controller supports RootController property.
 */
abstract class BaseParentController implements Controller {
	BaseParentController() {
	}

	/**
	 * Prints a warning message.
	 */
	protected final void warn(String v) {
		System.out.println("WARN: " + v);
	}

	/**
	 * Prints an info message.
	 */
	protected final void info(String v) {
		System.out.println("INFO: " + v);
	}

	/**
	 * Displays a message.
	 */
	protected final void trace(String v) {
	}

	/**
	 * Prints a debug message.
	 */
	public final void debug(String v) {
		if (isDebug()) {
			System.out.println("DEBUG: " + v);
		}
	}

	/**
	 * Gets Debug property.
	 */
	public final boolean isDebug() {
		return debugFlag;
	}

	/**
	 * Set Debug property.
	 */
	public final void setDebug(boolean v) {
		if (debugFlag == v) {
			return;
		}

		debugFlag = v;
		onDebug();
	}

	/**
	 * Debug event.
	 */
	protected abstract void onDebug();

	private boolean debugFlag;
}
