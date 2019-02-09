package fr.duvam.video;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import uk.co.caprica.vlcj.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.player.embedded.FullScreenStrategy;
import uk.co.caprica.vlcj.player.embedded.x.XFullScreenStrategy;

/**
 * An example of using the "X" full-screen strategy.
 * <p>
 * This is without doubt the recommended strategy to use for full-screen media
 * players - at least it is on Linux.
 */
public class VlcPlayer{

	private JFrame frame;
	private EmbeddedMediaPlayerComponent mediaPlayerComponent;

	public static void main(String[] args) {
		/*
		 * if(args.length != 1) { System.err.println("Specify an MRL to play");
		 * System.exit(1); }
		 */
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

		mediaPlayerComponent = new EmbeddedMediaPlayerComponent() {
			@Override
			protected FullScreenStrategy onGetFullScreenStrategy() {
				return new XFullScreenStrategy(frame);
			}
		};
		frame.setContentPane(mediaPlayerComponent);
		frame.setVisible(true);
	}

	protected void start(String mrl) {
		mediaPlayerComponent.getMediaPlayer().playMedia(mrl);
		mediaPlayerComponent.getMediaPlayer().setFullScreen(true);
		mediaPlayerComponent.getMediaPlayer().setRepeat(true);
	}
}
