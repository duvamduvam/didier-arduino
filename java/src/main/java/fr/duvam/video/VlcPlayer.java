package fr.duvam.video;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import fr.duvam.arduino.Arduino;
import fr.duvam.arduino.Communicator2;
import uk.co.caprica.vlcj.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;
import uk.co.caprica.vlcj.player.embedded.FullScreenStrategy;
import uk.co.caprica.vlcj.player.embedded.x.XFullScreenStrategy;

public class VlcPlayer{

	private JFrame frame;
	private EmbeddedMediaPlayer mediaPlayer;

	private Communicator2 communicator;
	private PlayerManager2 playerManager;
	private KeyListener2 listener;
	
	public static void main(String[] args) {

        MediaManager mediaManager = new MediaManager();

		final String mrl = mediaManager.getMedia(MediaManager.KEY_VIDEO_BASE);

		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new VlcPlayer().start(mrl);
			}
		});
	}

	@SuppressWarnings("serial")
	public VlcPlayer() {
	
		frame = new JFrame("LibX11 Full Screen Strategy");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocation(100, 100);
		frame.setSize(1200, 800);
		
		EmbeddedMediaPlayerComponent mediaPlayerComponent = new EmbeddedMediaPlayerComponent() {
			@Override
			protected FullScreenStrategy onGetFullScreenStrategy() {
				return new XFullScreenStrategy(frame);
			}
		};
		mediaPlayer = mediaPlayerComponent.getMediaPlayer();
		
		frame.setContentPane(mediaPlayerComponent);
		frame.setVisible(true);
		
		// Listener
		listener = new KeyListener2(mediaPlayer);
		PlayerManager2 playerManager = new PlayerManager2(mediaPlayer);
		
		playerManager.setKeyListener(listener);
		communicator = new Communicator2(listener);
		// end listener
		
	}

	protected void start(String mrl) {
		
		mediaPlayer.playMedia(mrl);
		mediaPlayer.setFullScreen(true);
		mediaPlayer.setRepeat(true);
		
		initListeners(listener);
	}
	
	/*private void initArduinoListener(KeyListener2 listener) {
		
        Thread thread = new Thread(listener);
		thread.setDaemon(true);
		thread.start();
	}	*/

	private void initListeners(KeyListener2 listener) {
		
		//arduino communicator
		Arduino arduino = new Arduino(listener, "/dev/ttyS4");
	    Thread arduinoThread = new Thread(arduino);
	    arduinoThread.setDaemon(true);
	    arduinoThread.start();
		
		//key listener
        Thread listenerThread = new Thread(listener);
        listenerThread.setDaemon(true);
        listenerThread.start();
	}	
	

	
}
