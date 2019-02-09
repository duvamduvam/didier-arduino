package fr.duvam.video;

import java.util.LinkedList;
import java.util.List;

import fr.duvam.arduino.Communicator2;
import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;

public class KeyListener2 implements Runnable {

	private EmbeddedMediaPlayer player;
	private List<String> keyList;
	MediaManager mediaManager;
	Communicator2 communicator;

	public KeyListener2(EmbeddedMediaPlayer player) {
		this.player = player;
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

				mediaManager.play(player, key);
				
				toRemove = key;
			}
			if (!toRemove.isEmpty()) {
				removeKey(toRemove);
			}
		}
	}

	@Override
	public void run() {
		while (true) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			checkEvent();
		}
	}
}
