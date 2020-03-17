package fr.duvam.video;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MediaListener implements Runnable {

	AudioPlayer audioPlayer;
	PlayerManager playerManager;
	CommandListener listener;

	private static Logger LOGGER = LoggerFactory.getLogger(MediaListener.class);

	public MediaListener(PlayerManager playerManager, CommandListener listener) {
		super();
		this.audioPlayer = new AudioPlayer();
		this.playerManager = playerManager;
	}

	private void checkRunning() {
		if (audioPlayer.isAudioFinised()) {
			playerManager.playDefaultVideo();
		}
	}

	@Override
	public void run() {
		while (true) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				LOGGER.error("erreur ArduinoListener", e);
			}
			if (audioPlayer.isAudioTriggered()) {
				checkRunning();
			}
			if (!playerManager.isPlaying() && !playerManager.isDefaultVideoPlaying()) {
				playerManager.playDefaultVideo();
			}
		}
	}

}
