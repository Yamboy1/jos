package org.jos.uritool;

/**
 * This tool encodes/decodes a string using URI encoding.
 */
public class URITool extends BaseURITool {
	/**
	 * Gets Tool property.
	 */
	public static URITool getTool() {
		if (tool == null) {
			tool = new URITool();
		}

		return tool;
	}

	private static URITool tool;

	/**
	 * Creates a tool.
	 */
	protected URITool() {
	}
}
