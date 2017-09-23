package org.jos.program2b.impl;

import java.util.Enumeration;

import org.jos.program2a.ParameterList;
import org.jos.program2b.ProgramContext;

/**
 * A basic program context is mostly read-only.
 */
public final class BasicProgramContext implements ProgramContext {
	private String path;
	private final ParameterList parameterList = new BasicParameterList();
	private int resultCode;

	/**
	 * Creates a program context.
	 */
	public BasicProgramContext() {
		super();
	}

	/**
	 * Gets Path property.
	 */
	@Override
	public String getPath() {
		return path;
	}

	/**
	 * Sets Path property. A program can get its own program-context; but, it
	 * should not easily set its own path.
	 */
	public void setPath(String v) {
		path = v;
	}

	/**
	 * Gets Parameter[] property.
	 */
	@Override
	public String getParameter(int index) {
		return parameterList.getParameter(index);
	}

	/**
	 * Gets Value[] property.
	 */
	@Override
	public String getValue(String key) {
		return parameterList.getValue(key);
	}

	/**
	 * Gets Values property.
	 */
	@Override
	public String[] getValues(String v) {
		return null;
	}

	/**
	 * Gets ResultCode property.
	 */
	@Override
	public int getResultCode() {
		return resultCode;
	}

	/**
	 * Sets ResultCode property.
	 */
	@Override
	public void setResultCode(int v) {
		resultCode = v;
	}

	/**
	 * Gets Parameters property.
	 */
	@Override
	public Enumeration<String> getParameters() {
		return parameterList.getParameters();
	}

	/**
	 * Parses arguments. A program can get its own program-context; but, it
	 * should not easily parse its own arguments.
	 */
	public void parseArguments(String v) {
		parameterList.parseArguments(v);
	}

	/**
	 * Parse arguments. A program can get its own program-context; but, it
	 * should not easily parse its own arguments.
	 */
	public void parseArguments(String[] v) {
		parameterList.parseArguments(v);
	}
}
