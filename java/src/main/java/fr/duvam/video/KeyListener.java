package fr.duvam.video;

import java.util.LinkedList;
import java.util.List;

import fr.duvam.arduino.Communicator;

public class KeyListener implements Runnable {

	private PlayerManager layout;
	private List<String> keyList;
	MediaManager mediaManager;
	Communicator communicator;

	public KeyListener(PlayerManager layout) {
		this.layout = layout;
		keyList = new LinkedList<String>();
		mediaManager = new MediaManager();
	}

	public void addKey(String key) {
		if (!keyList.contains(key) && mediaManager.existKey(key)) {
			keyList.add(key);
		}
	}

	private void removeKey(String key) {
		keyList.remove(key);
	}

	private synchronized void checkEvent() {
		synchronized (keyList) {
			String toRemove = new String();
			for (String key : keyList) {
				layout.getPlayer(key);
				toRemove = key;
			}
			if (!toRemove.isEmpty()) {
				removeKey(toRemove);
			}

		}
	}

	@Override
	public void run() {
		// while (true) {
		checkEvent();
		// }

	}
}
