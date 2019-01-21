package fr.duvam;

import java.awt.Canvas;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import uk.co.caprica.vlcj.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.discovery.NativeDiscovery;
import uk.co.caprica.vlcj.player.MediaPlayerEventAdapter;
import uk.co.caprica.vlcj.player.MediaPlayerFactory;
import uk.co.caprica.vlcj.player.embedded.DefaultFullScreenStrategy;
import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;
import uk.co.caprica.vlcj.player.embedded.FullScreenStrategy;
import uk.co.caprica.vlcj.player.embedded.videosurface.CanvasVideoSurface;

public class ReadVideo {

	private final JFrame frame;

	private final EmbeddedMediaPlayerComponent mediaPlayerComponent;

	public static void main(final String[] args) {
		new NativeDiscovery().discover();
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new ReadVideo(args);
			}
		});
	}

	public ReadVideo(String[] args) {
		frame = new JFrame("My First Media Player");
		frame.setBounds(100, 100, 600, 400);
		frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frame.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				mediaPlayerComponent.release();
				System.exit(0);
			}
		});

		String[] vlcArgs = { "aout=alsa" };
		//MediaPlayerFactory player = new MediaPlayerFactory(vlcArgs);

		mediaPlayerComponent = new EmbeddedMediaPlayerComponent();
		//mediaPlayerComponent.setStandardMediaOptions(vlcArgs);
		frame.setContentPane(mediaPlayerComponent);
		frame.setVisible(true);
		// EmbeddedMediaPlayer player = mediaPlayerComponent.getMediaPlayer();
		// player.set
		//.playMedia("/home/david/Nextcloud/robot/videos/base.mp4");
		EmbeddedMediaPlayer player = mediaPlayerComponent.getMediaPlayer();
		player.playMedia("/home/david/Nextcloud/robot/videos/base.mp4","aout=alsa");
	}

	public void readVideo2(){
		JFrame frame2 = new JFrame("My First Media Player");
		frame2.setBounds(100, 100, 600, 400);
		frame2.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frame2.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				mediaPlayerComponent.release();
				System.exit(0);
			}
		});
		
		
		   // Set some options for libvlc
		   String[] libvlcArgs = {"aout=alsa"};

		   // Create a factory
		   MediaPlayerFactory mediaPlayerFactory = new MediaPlayerFactory(libvlcArgs);

		   // Create a full-screen strategy
		   FullScreenStrategy fullScreenStrategy = new DefaultFullScreenStrategy(frame);

		   // Create a media player instance (in this example an embedded media player)
		   EmbeddedMediaPlayer mediaPlayer = mediaPlayerFactory.newEmbeddedMediaPlayer(fullScreenStrategy);

		   // Set standard options as needed to be applied to all subsequently played media items
		   String[] standardMediaOptions = {"video-filter=logo", "logo-file=vlcj-logo.png", "logo-opacity=25"};
		   mediaPlayer.setStandardMediaOptions(standardMediaOptions);

		   // Add a component to be notified of player events
		  // mediaPlayer.addMediaPlayerEventListener(new MediaPlayerEventAdapter() {...add implementation here...});

		   // Create and set a new component to display the rendered video (not shown: add the Canvas to a Frame)
		   Canvas canvas = new Canvas();
		   CanvasVideoSurface videoSurface = mediaPlayerFactory.newVideoSurface(canvas);
		   mediaPlayer.setVideoSurface(videoSurface);

		   // Play a particular item, with options if necessary
		   String mediaPath = "/home/david/Nextcloud/robot/videos/base.mp4";
		   //String[] mediaOptions = {...add options here...};
		   mediaPlayer.playMedia(mediaPath);

		   // Do some interesting things in the application
		  // ...

		   // Cleanly dispose of the media player instance and any associated native resources
		   mediaPlayer.release();

		   // Cleanly dispose of the media player factory and any associated native resources
		   mediaPlayerFactory.release();
	}
}