
package fr.duvam.video;

import java.io.File;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.MouseEvent;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.stage.Stage;

public class VideoPlayerExample extends Application {
	public static void main(String[] args) throws Exception {
		launch(args);
	}

	@Override
	public void start(final Stage stage) throws Exception {

		File file = new File("/home/david/Nextcloud/robot/videos/base.mp4");
		String MEDIA_URL = file.toURI().toString();
		// Media media = new Media(MEDIA_URL);

		final MediaPlayer oracleVid = new MediaPlayer(new Media(MEDIA_URL));

		Scene scene = new Scene(new Group(new MediaView(oracleVid)), 1024, 576);
		stage.setScene(scene);
		stage.show();

		oracleVid.setMute(true);
		oracleVid.setRate(20);

		oracleVid.setCycleCount(MediaPlayer.INDEFINITE);
		oracleVid.play();
		
		scene.setOnMouseClicked(new EventHandler <MouseEvent>(){
            @Override
            public void handle(MouseEvent clicked) {
            	oracleVid.stop();
            }
            
        });
		
		oracleVid.setOnEndOfMedia(new Runnable()
	        {
	            public void run()

	            {
	        		File file = new File("/home/david/Nextcloud/robot/videos/smile.mp4");
	        		String MEDIA_URL = file.toURI().toString();
	        		// Media media = new Media(MEDIA_URL);

	        		final MediaPlayer oracleVid2 = new MediaPlayer(new Media(MEDIA_URL));
	        		Scene scene = new Scene(new Group(new MediaView(oracleVid2)), 1024, 576);
	        		stage.setScene(scene);
	        		// oracleVid.set
	        		
	        		oracleVid2.setMute(true);
	        		oracleVid2.setRate(20);

	        		oracleVid2.play();
	            }
	        });
		
		oracleVid.setOnStopped(new Runnable()
        {
            public void run()

            {
        		File file = new File("/home/david/Nextcloud/robot/videos/bored.mp4");
        		String MEDIA_URL = file.toURI().toString();
        		// Media media = new Media(MEDIA_URL);

        		final MediaPlayer oracleVid2 = new MediaPlayer(new Media(MEDIA_URL));
        		Scene scene = new Scene(new Group(new MediaView(oracleVid2)), 1024, 576);
        		stage.setScene(scene);
        		// oracleVid.set
        		
        		oracleVid2.setMute(true);
        		oracleVid2.setRate(20);

        		oracleVid2.play();
            }
        });

		


	}
}