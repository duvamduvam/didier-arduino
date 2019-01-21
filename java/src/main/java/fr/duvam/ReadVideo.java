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

}
