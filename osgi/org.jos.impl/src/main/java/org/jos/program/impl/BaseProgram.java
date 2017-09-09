package org.jos.program.impl;

import org.jos.program.Program;
import org.jos.program.ProgramContext;

/**
 * This class should be useful as a base class for all your compatible programs.
 */
public abstract class BaseProgram implements Program {
	private ProgramContext context = null;

	/**
	 * Creates a program.
	 */
	protected BaseProgram() {
		super();
	}

	/**
	 * Gets Context property.
	 */
	public ProgramContext getProgramContext() {
		return context;
	}

	/**
	 * Sets Context property.
	 */
	public void setProgramContext(ProgramContext v) {
		context = v;
	}

	/**
	 * Gets Parameter[] property.
	 */
	public String getParameter(int v) {
		return getProgramContext().getParameter(v);
	}

	/**
	 * Gets Value[] property.
	 */
	public String getValue(String v) {
		return getProgramContext().getValue(v);
	}

	/**
	 * Gets ResultCode property.
	 */
	public int getResultCode() {
		return getProgramContext().getResultCode();
	}

	/**
	 * Sets ResultCode property.
	 */
	public void setResultCode(int v) {
		getProgramContext().setResultCode(v);
	}
}
