package fr.duvam.video;

import java.util.HashMap;
import java.util.Map;

import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;

public class MediaManager{

	Map<String, String> medias = new HashMap<String, String>();

	private final static String VIDEO_PATH = "videos/";
	private final static String VIDEO_EXTENSION = ".mp4";
	public final static String KEY_VIDEO_BASE = "10000";

	public MediaManager() {
		medias.put(KEY_VIDEO_BASE, "base");
		medias.put("10001", "bored");
		medias.put("10002", "smile");
		medias.put("10004", "anger");
	}

	public String getMedia(String key) {
		if (medias.containsKey(key)) {
			return getMediaPath(medias.get(key));
		}
		return getMediaPath(medias.get(KEY_VIDEO_BASE));
	}

	public boolean existKey(String key) {
		return medias.containsKey(key);
	}
	
	private String getMediaPath(String media) {
		return VIDEO_PATH + media + VIDEO_EXTENSION;
	}

	public void play(EmbeddedMediaPlayer player, String key) {
		String media = getMedia(key);
		player.stop();
		player.playMedia(media);
	}
	
}