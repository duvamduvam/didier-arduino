package fr.duvam.video;

import java.util.LinkedList;
import java.util.List;

public class Listener implements Runnable {

	private Layout layout;
	private List<String> eventList;

	public Listener(Layout layout) {
		this.layout = layout;
		eventList = new LinkedList<String>();
	}

	public void addEvent(String event) {
		eventList.add(event);
	}

	private void removeEvent(String event) {
		eventList.remove(event);
	}

	private synchronized void checkEvent() {
		synchronized (eventList) {
			String toRemove = new String();
			if (eventList.size() > 3) {
				for (String event : eventList) {
					layout.getPlayer("/home/david/Nextcloud/robot/videos/bored.mp4", true);
					toRemove = event;
				}
				if (!toRemove.isEmpty()) {
					removeEvent(toRemove);
				}
			}
		}
	}

	@Override
	public void run() {
		//while (true) {
			checkEvent();
		//}

	}
}
