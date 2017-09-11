package org.jos.gchii.exception1a;

/**
 * An exception tool formats a class name and message.
 */
public class ExceptionTool {
	/**
	 * Get the default tool.
	 */
	public static ExceptionTool getTool() {
		if (tool == null) {
			tool = new ExceptionTool();
		}

		return tool;
	}

	/**
	 * Set the default tool.
	 */
	public static void setTool(ExceptionTool v) {
		if (tool == null) {
			tool = v;
		}
	}

	private static ExceptionTool tool = null;

	/**
	 * Creates a tool.
	 */
	protected ExceptionTool() {
	}

	/**
	 * Formats an exception (Throwable).
	 */
	public String format(Throwable e) {
		return "[" + e.getClass().getName() + "] " + e.getMessage();
	}
}
