package fr.duvam.video;

import org.apache.log4j.Logger;

import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;

public class PlayerManager2 {

	private static final Logger LOGGER = Logger.getLogger(PlayerManager2.class);
	
	EmbeddedMediaPlayer player;
	
	KeyListener2 listener;

	boolean loadingNewVideo = false;

	MediaManager mediaManager;

	public PlayerManager2(EmbeddedMediaPlayer player) {
		this.player = player;
	}

	public void setKeyListener(KeyListener2 listener) {
		this.listener = listener;
	}
	
	public void playMedia(String key) {
		player.playMedia(key);

	}

	public EmbeddedMediaPlayer getPlayer() {
		return player;
	}

}