
package fr.duvam.video;

import java.io.File;

import fr.duvam.arduino.Communicator;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class VideoPlayer extends Application {

	// private MediaPlayer currentPlayer = null;
	private static Communicator communicator = null;

	public static void main(String[] args) throws Exception {
		launch(args);
	}

	@Override
	public void start(final Stage stage) throws Exception {

 
		// connect arduino
		communicator = new Communicator();
		communicator.searchForPorts();
		communicator.connect();
        if (communicator.getConnected() == true)
        {
            if (communicator.initIOStream() == true)
            {
                communicator.initListener();
            }
        }

        
		MediaPlayer player = getPlayer(stage, "/home/david/Nextcloud/robot/videos/base.mp4");
		player.setCycleCount(MediaPlayer.INDEFINITE);
		stage.initStyle(StageStyle.UNDECORATED);
		stage.show();

		/*player.setOnEndOfMedia(new Runnable() {
			public void run()

			{
				getPlayer(stage, "/home/david/Nextcloud/robot/videos/smile.mp4");
			}
		})*/;

		player.setOnStopped(new Runnable() {
			public void run()

			{
				getPlayer(stage, "/home/david/Nextcloud/robot/videos/bored.mp4");

			}
		});

	}

	private MediaPlayer getPlayer(Stage stage, String videoPath) {
		File file = new File(videoPath);
		String MEDIA_URL = file.toURI().toString();
		// Media media = new Media(MEDIA_URL);
		// Automatically begin the playback
		final MediaPlayer player = new MediaPlayer(new Media(MEDIA_URL));
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

		communicator.setPlayer(player);

		player.play();

		return player;
	}

	/*
	 * public MediaPlayer getCurrentPlayer() { return currentPlayer; }
	 * 
	 * public void setCurrentPlayer(MediaPlayer currentPlayer) { this.currentPlayer
	 * = currentPlayer; }
	 */
}