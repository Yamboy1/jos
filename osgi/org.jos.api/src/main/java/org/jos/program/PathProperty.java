package org.jos.program;

/**
 * This interface provides perfect backward compatibility for the servlet
 * program.
 */
public interface PathProperty {
	/**
	 * Gets Path property.
	 */
	public abstract String getPath();

	/**
	 * Sets Path property.
	 */
	public abstract void setPath(String v);
}
