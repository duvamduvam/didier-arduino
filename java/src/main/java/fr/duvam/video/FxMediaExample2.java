package fr.duvam.video;
import java.io.File;
import java.util.concurrent.TimeUnit;

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

//from https://examples.javacodegeeks.com/desktop-java/javafx/javafx-media-api/
public class FxMediaExample2 extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }

    @Override
    public void start(Stage stage) {
        // Create a Media
        File file = new File("/home/david/Nextcloud/robot/videos/base.mp4");
        String MEDIA_URL = file.toURI().toString();
        Media media = new Media(MEDIA_URL);

        // Create a Media Player
        MediaPlayer player = new MediaPlayer(media);
        // Automatically begin the playback
        player.setAutoPlay(true);

        MediaView mediaView = new MediaView(player);

        mediaView.setFitWidth(1024);
        mediaView.setFitHeight(576);

        VBox root = new VBox(0, mediaView);
        GridPane gridpane = new GridPane();
        Group grp = new Group();
        gridpane.add(root, 0, 0);

        grp.getChildren().add(mediaView);

        // Create the Scene
        Scene scene = new Scene(grp);

        // Add the scene to the Stage
        stage.setScene(scene);
        //remove titlebar	
        stage.initStyle(StageStyle.UNDECORATED);
        // Set the title of the Stage
        //stage.setTitle("A simple Media Example");
        // Display the Stage
        stage.show();
        
        
        /*try {
			TimeUnit.SECONDS.sleep(30);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
        
        file = new File("/home/david/Nextcloud/robot/videos/smile.mp4");
        MEDIA_URL = file.toURI().toString();
        Media newMedia = new Media(MEDIA_URL);
        player = new MediaPlayer(newMedia);
        mediaView = new MediaView(player);

        mediaView.setFitWidth(1024);
        mediaView.setFitHeight(576);

        root = new VBox(0, mediaView);
        gridpane = new GridPane();
        grp = new Group();
        gridpane.add(root, 0, 0);

        grp.getChildren().add(mediaView);

        // Create the Scene
        scene = new Scene(grp);

        // Add the scene to the Stage
        stage.setScene(scene);
        //remove titlebar	
        // Set the title of the Stage
        //stage.setTitle("A simple Media Example");
        // Display the Stage
        //stage.show();
        
    }
}
