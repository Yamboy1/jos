package net.sourceforge.jos;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

final class HtmlUpdateProgram implements Runnable {
	private static FileFilter html = new HtmlFileFilter();

	HtmlUpdateProgram() {
	}

	public void run() {
		try {
			println("Scanning...");

			File src = checkDirectory(pwd().getParentFile());
			src = cd(src, "jos-website");
			src = cd(src, "src");
			src = cd(src, "htdocs");
			src = cd(src, "html");
			println("src=" + src);

			File dest = new File(pwd(), "target");
			dest.mkdir();

			File[] list = src.listFiles(html);
			int iMax = list.length;
			for (int i = 0; i < iMax; i++) {
				File in = list[i];
				File out = new File(dest, in.getName());
				long count = copy(in, out);
				println("" + in + " count=" + count);
			}

			println("Done.");
		} catch (Throwable e) {
			_print(e);
		}
	}

	private long copy(File in, File out) throws IOException {
		BufferedReader is = null;
		try {
			is = new BufferedReader(new InputStreamReader(new FileInputStream(
					in)));
			return copy(is, out);
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Throwable e) {
					_print(e);
				}
			}
		}
	}

	private long copy(BufferedReader is, File out) throws IOException {
		PrintWriter os = null;
		try {
			os = new PrintWriter(new OutputStreamWriter(new FileOutputStream(
					out)));
			return copy(is, os);
		} finally {
			if (os != null) {
				try {
					os.close();
				} catch (Throwable e) {
					_print(e);
				}
			}
		}
	}

	private long copy(BufferedReader input, PrintWriter output)
			throws IOException {
		long count = 0;

		for (String line;; count++) {
			line = input.readLine();

			if (line == null) {
				break;
			}

			line = replaceLinks(line);
			line = replaceTags(line);
			line = replaceAttributes(line);

			output.println(line);
		}

		return count;
	}

	private String replaceLinks(final String line) {
		String r = line;

		// HREF="http://jos.sourceforge.net/">
		r = replaceAll(r, "HREF=\"http://jos.sourceforge.net/\">",
				"href=\"../wiki.html\">");

		// <a HREF="http://cjos.sourceforge.net/archive/info/">
		r = replaceAll(r,
				"<A HREF=\"http://cjos.sourceforge.net/archive/info/\">",
				"<a href=\"../info.html\">");

		// <a HREF="http://cjos.sourceforge.net/archive/dev/">
		r = replaceAll(r,
				"<A HREF=\"http://cjos.sourceforge.net/archive/dev/\">",
				"<a href=\"../dev.html\">");

		// <A HREF="http://cjos.sourceforge.net/archive/">
		r = replaceAll(r, "<A HREF=\"http://cjos.sourceforge.net/archive/\">",
				"<a href=\"../index.html\">");

		return r;
	}

	private String replaceTags(final String line) {
		String r = line;
		r = replaceTag("a", r);
		r = replaceTag("body", r);
		r = replaceTag("br", r);
		r = replaceTag("b", r);
		r = replaceTag("code", r);
		r = replaceTag("div", r);
		r = replaceTag("form", r);
		r = replaceTag("h1", r);
		r = replaceTag("h2", r);
		r = replaceTag("h3", r);
		r = replaceTag("h4", r);
		r = replaceTag("head", r);
		r = replaceTag("html", r);
		r = replaceTag("hr", r);
		r = replaceTag("img", r);
		r = replaceTag("li", r);
		r = replaceTag("meta", r);
		r = replaceTag("ol", r);
		r = replaceTag("pre", r);
		r = replaceTag("p", r);
		r = replaceTag("table", r);
		r = replaceTag("td", r);
		r = replaceTag("th", r);
		r = replaceTag("title", r);
		r = replaceTag("tr", r);
		r = replaceTag("ul", r);
		return r;
	}

	private String replaceTag(String tag, String line) {
		String r = line;
		r = replaceAll(r, "<" + tag.toUpperCase(), "<" + tag);
		r = replaceAll(r, "</" + tag.toUpperCase(), "</" + tag);
		return r;
	}

	private String replaceAttributes(final String line) {
		String r = line;
		r = replaceAttribute("width", r);
		r = replaceAttribute("vspace", r);
		r = replaceAttribute("valign", r);
		r = replaceAttribute("src", r);
		r = replaceAttribute("http-equiv", r);
		r = replaceAttribute("hspace", r);
		r = replaceAttribute("href", r);
		r = replaceAttribute("content", r);
		r = replaceAttribute("colspan", r);
		r = replaceAttribute("cellspacing", r);
		r = replaceAttribute("cellpadding", r);
		r = replaceAttribute("border", r);
		r = replaceAttribute("bgcolor", r);
		r = replaceAttribute("align", r);
		r = replaceAttribute("action", r);
		return r;
	}

	private String replaceAttribute(final String attribute, final String line) {
		int ltPos;
		{
			ltPos = line.indexOf("<");
			if (ltPos < 0) {
				return line;
			}
		}

		int gtPos;
		{
			gtPos = line.indexOf(">", ltPos);
			if (gtPos < 0) {
				return line;
			}
		}

		String oldWord = " " + attribute.toUpperCase() + "=\"";
		int pos = line.indexOf(oldWord);

		// println("attribute='" + attribute + "' line='" + line + "'");
		
		if (pos < 0) {
		} else {
			if (ltPos < pos && pos < gtPos) {
				String prefix = line.substring(0, pos);
				String suffix = replaceAttribute(attribute, line.substring(pos
						+ oldWord.length()));

				String r = prefix + " " + attribute + "=\"" + suffix;

				if (attribute.equals("align") || attribute.equals("href")) {
					// println("pos=" + pos + " ltPos=" + ltPos + " gtPos="	+ gtPos);
				}

				// println("-->" + r);
				return r;
			}
		}

		String prefix = line.substring(0, gtPos + 1);
		String suffix = replaceAttribute(attribute, line.substring(gtPos + 1));
		String r = prefix + suffix;
		// println("==>" + r);
		return r;
	}

	private String replaceAll(String text, String oldWord, String newWord) {
		if (text.length() < oldWord.length()) {
			return text;
		}

		int pos = text.indexOf(oldWord);
		// println("pos=" + pos + " text=" + text + " oldWord=" + oldWord +
		// " newWord=" + newWord);

		if (pos < 0) {
			return text;
		}

		String prefix = text.substring(0, pos);
		String suffix = replaceAll(text.substring(pos + oldWord.length()),
				oldWord, newWord);
		return prefix + newWord + suffix;
	}

	private File pwd() {
		File r = new File(".").getAbsoluteFile().getParentFile();
		return checkDirectory(r);
	}

	private File cd(File d, String path) {
		File r = new File(d, path);
		return checkDirectory(r);
	}

	private File checkDirectory(File r) {
		if (!r.exists()) {
			throw new RuntimeException("'" + r + "' directory is missing.");
		}

		if (!r.isDirectory()) {
			throw new RuntimeException("'" + r + "' is not a directory.");
		}

		return r;
	}

	private void println(String v) {
		System.out.println("[HTML Update Program]" + v);
	}

	private void _print(Throwable e) {
		System.err.println("[HTML Update Program][" + e.getClass().getName()
				+ "] " + e.getMessage());
	}
}
