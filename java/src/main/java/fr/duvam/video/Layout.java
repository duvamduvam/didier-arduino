package fr.duvam.video;

import java.io.File;

import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.stage.Stage;

public class Layout {

	Stage stage;
	MediaPlayer player;
	
	File file;
	String MEDIA_URL;
	MediaView mediaView;
	VBox root;
	GridPane gridpane;
	Group grp;
	Scene scene;

	public Layout(Stage stage) {
		super();
		this.stage = stage;
		// TODO Auto-generated constructor stub
	}

	public MediaPlayer getPlayer(String videoPath, Boolean loop) {
		if (player != null) {
			player.stop();
			player.dispose();
		}

		//check number of thread
		System.out.println("number of thread : "+java.lang.Thread.activeCount());
		
		file = new File(videoPath);
		MEDIA_URL = file.toURI().toString();
		// Media media = new Media(MEDIA_URL);
		// Automatically begin the playback
		player = new MediaPlayer(new Media(MEDIA_URL));
		// oracleVid.setAutoPlay(true);

		mediaView = new MediaView(player);

		mediaView.setFitWidth(1024);
		mediaView.setFitHeight(576);

		root = new VBox(0, mediaView);
		gridpane = new GridPane();
		grp = new Group();
		gridpane.add(root, 0, 0);

		grp.getChildren().add(mediaView);
		scene = new Scene(grp);
		stage.setScene(scene);

		player.setMute(true);
		player.setRate(20);

		if (loop) {
			player.setCycleCount(MediaPlayer.INDEFINITE);
		}

		player.play();

		return player;
	}

}
