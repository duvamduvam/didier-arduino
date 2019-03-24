package fr.duvam.video;

import java.io.File;

import javax.swing.JFrame;

import org.apache.log4j.Logger;

import fr.duvam.video.MediaManager.Control;
import fr.duvam.video.MediaManager.Type;
import uk.co.caprica.vlcj.component.AudioMediaPlayerComponent;
import uk.co.caprica.vlcj.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.player.MediaPlayer;
import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;
import uk.co.caprica.vlcj.player.embedded.FullScreenStrategy;
import uk.co.caprica.vlcj.player.embedded.x.XFullScreenStrategy;
import uk.co.caprica.vlcj.player.media.Media;

public class PlayerManager {

	private static final Logger LOGGER = Logger.getLogger(PlayerManager.class);

	private EmbeddedMediaPlayer videoPlayer;
	private MediaPlayer audioPlayer;
	//private Clip audioPlayer;

	private MediaManager mediaManager;

	private boolean isAudioTriggeredButNotStarted = false;
	private boolean isAudioTriggered = false;

	public PlayerManager(MediaManager mediaManager) {

		this.mediaManager = mediaManager;

		initVideo();
		initAudio();
	}

	private void initVideo() {
		////////// video
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocation(100, 100);
		frame.setSize(1200, 800);

		EmbeddedMediaPlayerComponent videoPlayerComponent = new EmbeddedMediaPlayerComponent() {
			@Override
			protected FullScreenStrategy onGetFullScreenStrategy() {
				return new XFullScreenStrategy(frame);
			}
		};
		videoPlayer = videoPlayerComponent.getMediaPlayer();

		frame.setContentPane(videoPlayerComponent);
		frame.setVisible(true);
	}

	private void initAudio() {
		AudioMediaPlayerComponent audioPlayerComponent = new AudioMediaPlayerComponent();
		audioPlayer = audioPlayerComponent.getMediaPlayer();
		audioPlayer.setVolume(200);

	}

	public EmbeddedMediaPlayer getVideoPlayer() {
		return videoPlayer;
	}

	public void playVideoKey(String key) {
		String video = mediaManager.getVideo(key);
		videoPlayer.stop();
		videoPlayer.playMedia(video);
	}

	public void playVideo(String video) {
		videoPlayer.stop();
		videoPlayer.playMedia(video);
	}

	public void playDefaultVideo() {
		final String defaultVideo = mediaManager.getVideo(MediaManager.KEY_VIDEO_BASE);
		getVideoPlayer().setFullScreen(true);
		getVideoPlayer().setRepeat(true);
		playVideo(defaultVideo);
	}

	public void stopVideo() {
		videoPlayer.stop();
	}

	public boolean isVideoPlaying() {
		return videoPlayer.isPlaying();
	}

	public void play(String key) {
		Type type = mediaManager.getControlType(key);
		switch (type) {
		case VIDEO:
			String video = mediaManager.getVideo(key);
			playVideo(video);
			break;
		case SPEAK:
			Control control = mediaManager.getControl(key);
			speakControl(control);
		}
	}

	public boolean isAudioPlaying() {
		boolean isPlaying = audioPlayer.isPlaying();
		if (isPlaying) {
			isAudioTriggeredButNotStarted = false;
		}
		return (isPlaying || isAudioTriggeredButNotStarted);
	}

	public boolean isAudioFinised() {
		boolean isPlaying = isAudioPlaying();
		boolean finished = isAudioTriggered && !isPlaying;
		if (finished && isAudioTriggered) {
			isAudioTriggered = false;
		}
		return finished;
	}

	public boolean isAudioTriggered() {
		return isAudioTriggered;
	}

	private void playAudio(String src) {
		isAudioTriggeredButNotStarted = true;
		isAudioTriggered = true;
		audioPlayer.stop();
		audioPlayer.prepareMedia(src);
		audioPlayer.play();
	}

	public void speakAudio(String audio) {
		final String video = mediaManager.getVideo(MediaManager.KEY_VIDEO_SPEAK);
		playVideo(video);
		playAudio(audio);
	}

	public void speakControl(Control control) {
		final String video = mediaManager.getVideo(MediaManager.KEY_VIDEO_SPEAK);
		playVideo(video);
		String audio = mediaManager.getAudioNavigation(control);
		playAudio(audio);
	}

}