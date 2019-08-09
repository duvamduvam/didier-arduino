package fr.duvam.video;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

import org.apache.log4j.Logger;

//test keys without the remote control via a texte file
public class TestKeyProvider{

	private static final Logger LOGGER = Logger.getLogger(TestKeyProvider.class);
	private static String keyFile = "keys/keys.txt";
	private CommandListener listener;

	public TestKeyProvider(CommandListener listener) {
		this.listener = listener;
	}

	public void checkTestInputFile() {

		ArrayList<String> list = new ArrayList<String>();

		try {
			File file = new File(keyFile);

			Scanner s = new Scanner(file);
			boolean hasContent = false;
			while (s.hasNext()) {
				hasContent = true;
				list.add(s.next());
			}
			s.close();

			// delete content
			if (file.exists() && hasContent) {
				file.delete();
				try {
					file.createNewFile();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

		} catch (IOException e) {

			e.printStackTrace();
		}

		for (Iterator<String> i = list.iterator(); i.hasNext();) {
			fireKey(i.next());
		}

	}

	private void fireKey(String key, int waitSecond) {
		try {
			TimeUnit.SECONDS.sleep(waitSecond);
		} catch (InterruptedException e) {
			LOGGER.error(e);
		}

		listener.addKey(key);
	}

	private void fireKey(String key) {
		fireKey(key, 5);
	}

	/*@Override
	public void run() {
		while (true) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				LOGGER.error("erreur check keys file", e);
			}
			checkFile();
		}
	}*/

}
