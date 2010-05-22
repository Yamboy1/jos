package net.sourceforge.jos;

public final class Main {
	public static void main(String[] args) {
		Runnable program = new HtmlUpdateProgram();
		program.run();
	}

	private Main() {
		throw new RuntimeException("do not instantiate");
	}
}
