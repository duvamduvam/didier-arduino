package fr.duvam;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.SwingUtilities;

import org.apache.log4j.Logger;

import fr.duvam.arduino.test.ArduinoComm;
import fr.duvam.midi.MidiHandler;
import fr.duvam.video.CommandListener;
import fr.duvam.video.MediaListener;
import fr.duvam.video.MediaManager;
import fr.duvam.video.PlayerManager;

public class Main {

	private static Logger LOGGER;
	
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

		initLog();
		
		MediaManager mediaManager = new MediaManager();
		listener = new CommandListener(playerManager, mediaManager);
		playerManager = new PlayerManager(mediaManager, listener);
		listener.setPlayerManager(playerManager);
		new MidiHandler(listener);
		
		
	}

	
	void initLog(){
		//init log4j property logging
		try {
			String logPrefix = OSValidator.getOS()+"-"+InetAddress.getLocalHost().getHostName();			
			System.setProperty("hostName", logPrefix);
			Logger LOGGER = Logger.getLogger(Main.class);
			LOGGER.info("<<<<<<<<<<<<<<<<<<<<<<<<<<< START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		} catch (UnknownHostException e) {
			LOGGER.error(e);
		} 
	}
	
	protected void start() {

		playerManager.playDefaultVideo();
		initListeners(listener);
	}

	private void initListeners(CommandListener keyListener) {

		// arduino communicator
		//ArduinoComm arduino = new ArduinoComm(keyListener);
		//Thread arduinoThread = new Thread(arduino);
		//arduinoThread.setDaemon(true);
		//arduinoThread.start();

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
