package fr.duvam.video;

import java.awt.Component;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.Paths;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

import org.apache.log4j.Logger;

import fr.duvam.video.MediaManager.Type;
import uk.co.caprica.vlcj.player.base.MediaPlayer;
import uk.co.caprica.vlcj.player.base.MediaPlayerEventAdapter;
import uk.co.caprica.vlcj.player.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;
import uk.co.caprica.vlcj.player.embedded.fullscreen.adaptive.AdaptiveFullScreenStrategy;

public class PlayerManager {

	private static final Logger LOGGER = Logger.getLogger(PlayerManager.class);

	private boolean defaultVideoPlaying = true;
	private boolean isPlaying = false;

	private EmbeddedMediaPlayer videoPlayer;
	private EmbeddedMediaPlayerComponent videoPlayerComponent;
	private AudioPlayer audioPlayer;
	// private Clip audioPlayer;
	private JFrame frame;

	private final MediaManager mediaManager;

	public PlayerManager(MediaManager mediaManager, CommandListener commandListener) {

		this.mediaManager = mediaManager;

		frame = new JFrame();
		// frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocation(100, 100);
		frame.setSize(1200, 800);

		frame.addKeyListener(new KeyboardListener(commandListener));
		
		initVideo();
		initAudio();
	}

	public boolean isDefaultVideoPlaying() {
		return defaultVideoPlaying;
	}

	public void setDefaultVideoPlaying(boolean defaultVideoPlaying) {
		this.defaultVideoPlaying = defaultVideoPlaying;
	}

	public boolean isPlaying() {
		return isPlaying;
	}

	public void setPlaying(boolean isPlaying) {
		this.isPlaying = isPlaying;
	}

	private void initVideo() {
		////////// video

		videoPlayerComponent = new EmbeddedMediaPlayerComponent(null, null,
				new AdaptiveFullScreenStrategy(frame), null, null);

		
		videoPlayer = videoPlayerComponent.mediaPlayer();

		videoPlayer.events().addMediaPlayerEventListener(new MediaPlayerEventAdapter() {
			@Override
			public void finished(final MediaPlayer mediaPlayer) {
				isPlaying = false;
				//exit(0);
			}
           /* @Override
            public void error(MediaPlayer mediaPlayer) {
                exit(1);
            }*/
		});

		frame.setContentPane(videoPlayerComponent);
		frame.setVisible(true);
	}

    private void exit(int result) {
    	videoPlayerComponent.release();
        System.exit(result);
    }
	
	private void initAudio() {
		// AudioMediaPlayerComponent audioPlayerComponent = new
		// AudioMediaPlayerComponent();
		// audioPlayer = audioPlayerComponent.getMediaPlayer();
		// audioPlayer.setVolume(200);
		audioPlayer = new AudioPlayer();

	}

	public EmbeddedMediaPlayer getVideoPlayer() {
		return videoPlayer;
	}

	public void playVideoKey(String key) {
		String video = mediaManager.getVideo(key);
		// Media media = new Media();

		stopVideo();
		// videoPlayer.playMedia(media)

		getVideoPlayer().media().play(video);
	}

	public void playDefaultVideo() {
		final String defaultVideo = mediaManager.getVideo(MediaManager.KEY_VIDEO_BASE);
		// getVideoPlayer().setFullScreen(true);
		// getVideoPlayer().controls().setRepeat(repeat);
		playVideo(defaultVideo, true);
		defaultVideoPlaying = true;
	}

	public void stopVideo() {
		getVideoPlayer().controls().stop();
		videoPlayerComponent.release();
	}

	// public boolean isVideoPlaying() {
	// return videoPlayer.
	// }

	public void play(String key) {
		
		LOGGER.info("fire : " + key);
		RositaMedia media = mediaManager.getMedia(key);
		Type type = media.getType();

		if (type == null)
			return;
		switch (type) {
		case GIF:
			// TODO make full screen integrated to jpane
			playGIF(media.getVideo());
			break;
		case VIDEO:
			String video = mediaManager.getVideo(key);
			playVideo(video, false);
			break;
		case SPEAK:
			speak(media.getSound());
			break;
		case AUDIO_VIDEO:
			playAudioVideo(media.getSound(), media.getVideo());
			break;
		}
		defaultVideoPlaying = false;
	}

	public void speakAudio(String audio) {
		final String video = mediaManager.getVideo(MediaManager.KEY_VIDEO_SPEAK);
		playAudioVideo(audio, video);
	}

	public void playAudioVideo(String audio, String video) {
		playVideo(video, false);
		audioPlayer.play(audio);

	}

	public void speak(String audio) {
		speakAudio(audio);
	}

	public void playVideo(String video, boolean repeat) {
		stopVideo();
		initVideo();
		getVideoPlayer().controls().setRepeat(repeat);
		videoPlayer.media().play(video);
		videoPlayer.fullScreen().set(true);
		isPlaying = true;
	}

	public void playGIF(String gif) {
		stopVideo();
		try {
			JFrame frame2 = new JFrame();
			URL url = Paths.get(gif).toUri().toURL();

			ImageIcon imageIcon = new ImageIcon(url);
			// JLabel label = new JLabel(imageIcon);

			// frame.getContentPane().add(label);
			// frame.setContentPane(label);
			// frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			// frame.pack();
			// frame.setLocationRelativeTo(null);
			// frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
			// frame.setUndecorated(true);
			// frame.setVisible(true);

			// Icon imgIcon = new ImageIcon(this.getClass().getResource("ajax-loader.gif"));
			JLabel label = new JLabel(imageIcon);
			// label.setBounds(668, 43, 46, 14); // You can use your own values
			frame2.getContentPane().add(label);
			frame2.pack();
			frame2.setLocationRelativeTo(null);
			frame2.setExtendedState(JFrame.MAXIMIZED_BOTH);
			// frame2.setUndecorated(true);
			frame2.setVisible(true);

		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public AudioPlayer getAudioPlayer() {
		return audioPlayer;
	}

	public void setAudioPlayer(AudioPlayer audioPlayer) {
		this.audioPlayer = audioPlayer;
	}

}