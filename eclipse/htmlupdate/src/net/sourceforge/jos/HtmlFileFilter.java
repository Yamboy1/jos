package net.sourceforge.jos;

import java.io.File;
import java.io.FileFilter;

final class HtmlFileFilter implements FileFilter {
	HtmlFileFilter() {
	}

	public boolean accept(File f) {
		if (!f.exists()) {
			return false;
		}
		
		if (!f.isFile()) {
			return false;
		}
		
		return f.getName().toLowerCase().endsWith(".html"); 
	}
	
	
}
