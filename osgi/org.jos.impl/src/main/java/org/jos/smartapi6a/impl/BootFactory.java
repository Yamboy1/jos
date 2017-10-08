package org.jos.smartapi6a.impl;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.jos.smartapi6a.spi.BaseFactory;

/**
 * This class is only used during start-up to load the default plug-in. A boot
 * factory is discarded. It is not added to the list of plug-ins. It does not
 * participate in any other request. In turn, the default plug-in is responsible
 * for loading all plug-ins and handling all requests.
 */
public class BootFactory extends BaseFactory {
	/**
	 * Creates a factory.
	 */
	public BootFactory() {
		trace("constructor()");

		BufferedReader reader = null;
		try {
			String name = "bootstrap.txt";
			debug("Reading from " + name + " resource...");
			reader = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream(name)));

			for (int i = 0;; i++) {
				String line = reader.readLine();
				if (line == null) {
					break;
				}

				debug("[" + i + "] " + line);

				if (line.startsWith(";")) {
					continue;
				}

				addControllerDescriptor(line);
			}
		} catch (Throwable ignore) {
		} finally {
			try {
				reader.close();
			} catch (Throwable ignore) {
			}
		}
	}
}
