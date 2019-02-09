package fr.duvam.video;

import org.apache.log4j.Logger;

import javafx.scene.media.MediaPlayer;

public class OnStopRunnable implements Runnable {
	PlayerManager playerManger;
	KeyListener keyListener;

	private static final Logger LOGGER = Logger.getLogger(OnStopRunnable.class);
	
	
	public OnStopRunnable(KeyListener listener, PlayerManager playerManager) {
		super();
		this.keyListener = listener;
		this.playerManger = playerManager;
	}

	@Override
	public void run() {
		MediaPlayer mediaPlayer = playerManger.getPlayer();
		LOGGER.debug("check player stop, cycle count : "+mediaPlayer.getCycleCount());
		if (mediaPlayer.getCycleCount() == 0) {
			LOGGER.debug("send key video base");
			keyListener.addKey(MediaManager.KEY_VIDEO_BASE);
		}
	}
}
