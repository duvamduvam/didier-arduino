package fr.duvam.video;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class MediaManager {

	// private static final Logger LOGGER = Logger.getLogger(MediaManager.class);

	Map<String, String> videos = new HashMap<String, String>();

	List<String> audios = new LinkedList<String>();

	private final static String AUDIO_PATH = "audios/";
	private final static String VIDEO_PATH = "videos/";

	private final static String VIDEO_EXTENSION = ".mp4";
	public final static String KEY_VIDEO_BASE = "10000";
	public final static String KEY_VIDEO_SPEAK = "10001";

	enum Type {
		VIDEO, SPEAK
	}

	enum Control {
		NEXT("10009"), CURRENT("10007"), PREVIOUS("10008");

		Control(String key) {
			this.key = key;
		}

		private final String key;

		public String getKey() {
			return key;
		}
	}

	private int currentAudio = 0;

	public MediaManager() {
		videos.put(KEY_VIDEO_BASE, "rbase");
		videos.put(KEY_VIDEO_SPEAK, "rspeak");

		videos.put("10002", "smile");
		videos.put("10004", "anger");
		videos.put("10005", "rspeak");

		loadAudios();
	}

	private void loadAudios() {
		File[] files = new File("audios").listFiles();
		// If this pathname does not denote a directory, then listFiles() returns null.

		for (File file : files) {
			if (file.isFile()) {
				audios.add(file.getName());
			}
		}
		java.util.Collections.sort(audios);
	}

	public Type getControlType(String key) {
		if (videos.containsKey(key)) {
			return Type.VIDEO;
		} else {
			for (Control control : Control.values()) {
				if (control.getKey().equals(key)) {
					return Type.SPEAK;
				}
			}
		}
		return null;
	}

	public Control getControl(String key) {

		for (Control control : Control.values()) {
			if (control.getKey().equals(key)) {
				return control;
			}
		}
		return null;
	}

	public String getVideo(String key) {

		if (videos.containsKey(key)) {
			return getVideoPath(videos.get(key));
		}
		return getVideoPath(videos.get(KEY_VIDEO_BASE));
	}

	public boolean existVideoKey(String key) {
		return videos.containsKey(key);
	}

	private String getVideoPath(String media) {
		return VIDEO_PATH + media + VIDEO_EXTENSION;
	}

	public String getAudioNavigation(Control control) {
		String audio = null;
		switch (control) {
		case NEXT:
			if ((currentAudio +1)< audios.size()) {
				audio = audios.get(++currentAudio);
			}
			break;
		case CURRENT:
			if (currentAudio <= audios.size()) {
				audio = audios.get(currentAudio);
			}
			break;
		case PREVIOUS:
			if (currentAudio > 0) {
				audio = audios.get(--currentAudio);
			} else {
				audio = audios.get(currentAudio);
			}
		}
		return AUDIO_PATH + audio;
	}
}