package fr.duvam.video;

import org.apache.log4j.Logger;

import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;

public class PlayerManager {

	private static final Logger LOGGER = Logger.getLogger(PlayerManager.class);
	
	EmbeddedMediaPlayer player;
	
	KeyListener listener;

	boolean loadingNewVideo = false;

	MediaManager mediaManager;

	public PlayerManager(EmbeddedMediaPlayer player) {
		this.player = player;
	}

	public void setKeyListener(KeyListener listener) {
		this.listener = listener;
	}
	
	public void playMedia(String key) {
		player.playMedia(key);

	}

	public EmbeddedMediaPlayer getPlayer() {
		return player;
	}

}