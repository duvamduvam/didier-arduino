package fr.duvam;

import javax.swing.SwingUtilities;

import fr.duvam.arduino.Arduino;
import fr.duvam.video.AudioListener;
import fr.duvam.video.KeyListener;
import fr.duvam.video.MediaManager;
import fr.duvam.video.PlayerManager;

public class Main {

	private KeyListener listener;
	private PlayerManager playerManager;

	public static void main(String[] args) {

		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new Main().start();
			}
		});
	}

	public Main() {

		MediaManager mediaManager = new MediaManager();
		playerManager = new PlayerManager(mediaManager);
		listener = new KeyListener(playerManager, mediaManager);
		
	}

	protected void start() {

		playerManager.playDefaultVideo();


		
		//audioPlayer.playMedia("/home/david/tmp/yom.mp3");
		// audioPlayer.setRepeat(true);
		//mediaManager.playMusic("/home/david/tmp/yom.mp3");
		
		initListeners(listener);
	}

	private void initListeners(KeyListener listener) {

		// arduino communicator
		Arduino arduino = new Arduino(listener);
		Thread arduinoThread = new Thread(arduino);
		arduinoThread.setDaemon(true);
		arduinoThread.start();

		// key listener
		Thread keyListenerThread = new Thread(listener);
		keyListenerThread.setDaemon(true);
		keyListenerThread.start();
		
		// audio listener
		AudioListener audioListener = new AudioListener(playerManager);
		Thread audioListenerThread = new Thread(audioListener);
		audioListenerThread.setDaemon(true);
		audioListenerThread.start();
		
	}

}
