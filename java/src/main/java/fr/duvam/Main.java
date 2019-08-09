package fr.duvam;

import javax.swing.SwingUtilities;

import fr.duvam.arduino.Arduino;
import fr.duvam.video.CommandListener;
import fr.duvam.video.MediaListener;
import fr.duvam.video.MediaManager;
import fr.duvam.video.PlayerManager;

public class Main {

	private CommandListener listener;
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
		listener = new CommandListener(playerManager, mediaManager);
		playerManager = new PlayerManager(mediaManager, listener);
		listener.setPlayerManager(playerManager);
	}

	protected void start() {

		playerManager.playDefaultVideo();


		
		//audioPlayer.playMedia("/home/david/tmp/yom.mp3");
		// audioPlayer.setRepeat(true);
		//mediaManager.playMusic("/home/david/tmp/yom.mp3");
		
		initListeners(listener);
	}

	private void initListeners(CommandListener keyListener) {

		// arduino communicator
		Arduino arduino = new Arduino(keyListener);
		Thread arduinoThread = new Thread(arduino);
		arduinoThread.setDaemon(true);
		arduinoThread.start();

		// key listener
		Thread keyListenerThread = new Thread(keyListener);
		keyListenerThread.setDaemon(true);
		keyListenerThread.start();
		
		// audio listener
		/*AudioListener audioListener = new AudioListener(playerManager);
		Thread audioListenerThread = new Thread(audioListener);
		audioListenerThread.setDaemon(true);
		audioListenerThread.start();*/

		// video listener
		MediaListener mediaListener = new MediaListener(playerManager, keyListener);
		Thread mediaListenerThread = new Thread(mediaListener);
		mediaListenerThread.setDaemon(true);
		mediaListenerThread.start();		
		
		// test key listener
		/*TestKeyProvider testKeyListener = new TestKeyProvider(listener);
		Thread testKeyListenerThread = new Thread(testKeyListener);
		testKeyListenerThread.setDaemon(true);
		testKeyListenerThread.start();	*/	
		
	}

}
