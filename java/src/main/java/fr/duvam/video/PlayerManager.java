package fr.duvam.video;

import java.io.File;

import org.apache.log4j.Logger;

import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.stage.Stage;

public class PlayerManager {

	private static final Logger LOGGER = Logger.getLogger(PlayerManager.class);
	
	Stage stage;

	MediaPlayer player;
	
	KeyListener listener;

	boolean loadingNewVideo = false;

	MediaManager mediaManager;

	public PlayerManager(Stage stage) {
		super();
		this.stage = stage;
		mediaManager = new MediaManager();
	}

	public void setKeyListener(KeyListener listener) {
		this.listener = listener;
	}
	
	public void getPlayer(String key) {
		if (player != null) {
			player.stop();
			player.dispose();
		}

		String videoPath = mediaManager.getMedia(key);
		LOGGER.info("change video : " + videoPath);
		File file = new File(videoPath);

		String MEDIA_URL = file.toURI().toString();
		// Media media = new Media(MEDIA_URL);
		// Automatically begin the playback
		player = new MediaPlayer(new Media(MEDIA_URL));
		// oracleVid.setAutoPlay(true);

		MediaView mediaView = new MediaView(player);

		mediaView.setFitWidth(1024);
		mediaView.setFitHeight(576);

		VBox root = new VBox(0, mediaView);
		

		
		GridPane gridpane = new GridPane();
		Group grp = new Group();
		gridpane.add(root, 0, 0);

		grp.getChildren().add(mediaView);
		Scene scene = new Scene(grp);
		stage.setScene(scene);
		
		player.setMute(true);
		player.setRate(20);

		if (MediaManager.KEY_VIDEO_BASE.equals(key)) {
			player.setCycleCount(MediaPlayer.INDEFINITE);
		} else {
			player.setCycleCount(5);
		}

		player.play();

		player.setOnStopped(new OnStopRunnable(listener, this));

	}

	public MediaPlayer getPlayer() {
		return player;
	}

}
