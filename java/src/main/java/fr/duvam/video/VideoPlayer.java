
package fr.duvam.video;

import java.io.File;

import fr.duvam.arduino.Communicator;
import javafx.application.Application;
import javafx.application.Platform;
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

	private Communicator communicator;

	public static void main(String[] args) throws Exception {
		launch(args);
	}

	@Override
	public void start(final Stage stage) throws Exception {

		Layout layout = new Layout(stage);
		Listener listener = new Listener(layout);
		
		 Thread thread = new Thread(new Runnable() {

	            @Override
	            public void run() {
	                while (true) {
	                    try {
	                        Thread.sleep(1000);
	                    } catch (InterruptedException ex) {
	                    }

	                    // UI update is run on the Application thread
	                    Platform.runLater(listener);
	                }
	            }
		 });
		 
		 
		//Platform.runLater(listener);
		//listener.start();
		// connect arduino
		communicator = new Communicator(listener);
		communicator.searchForPorts();
		communicator.connect();
		if (communicator.getConnected() == true) {
			if (communicator.initIOStream() == true) {
				communicator.initListener();
			}
		}

		layout.getPlayer("/home/david/Nextcloud/robot/videos/base.mp4", true);

		
        thread.setDaemon(true);
        thread.start();
		
		stage.initStyle(StageStyle.UNDECORATED);
		stage.show();

		/*
		 * WatchService watchService = FileSystems.getDefault().newWatchService();
		 * 
		 * Path path = Paths.get("/home/david/tmp/");
		 * 
		 * path.register(watchService, StandardWatchEventKinds.ENTRY_CREATE);
		 * 
		 * 
		 * //test listener
		 * 
		 * WatchKey key; while ((key = watchService.take()) != null) { for
		 * (WatchEvent<?> event : key.pollEvents()) {
		 * System.out.println("Event new video");
		 * layout.getPlayer("/home/david/Nextcloud/robot/videos/angry.mp4", true); }
		 * key.reset(); }
		 */

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

		// communicator.setPlayer(player);

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