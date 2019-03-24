package fr.duvam.video;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class AudioListener implements Runnable {

	private static Logger LOGGER = LoggerFactory.getLogger(AudioListener.class);

	private PlayerManager playerManager;

	public AudioListener(PlayerManager playerManager) {
		super();
		this.playerManager = playerManager;
	}

	private void checkRunning() {
		if (playerManager.isAudioFinised()) {
			playerManager.playDefaultVideo();
		}
	}

	@Override
	public void run() {
		while (true) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				LOGGER.error("erreur AudioListener", e);
			}
			if (playerManager.isAudioTriggered()) {
				checkRunning();
			}
		}
	}

}
