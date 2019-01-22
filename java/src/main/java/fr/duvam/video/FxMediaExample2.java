package fr.duvam.video;
import java.io.File;

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

public class FxMediaExample2 extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }

    @Override
    public void start(Stage stage) {
        // Locate the media content in the CLASSPATH
        //URL mediaUrl = getClass().getResource("/home/david/Nextcloud/robot/videos/base.mp4");
        //String mediaStringUrl = mediaUrl.toExternalForm();

        // Create a Media
        
        File file = new File("/home/david/Nextcloud/robot/videos/base.mp4");
        String MEDIA_URL = file.toURI().toString();
        Media media = new Media(MEDIA_URL);

        // Create a Media Player
        final MediaPlayer player = new MediaPlayer(media);
        // Automatically begin the playback
        player.setAutoPlay(true);

        // Create a 400X300 MediaView
        MediaView mediaView = new MediaView(player);

        mediaView.setFitWidth(1024);
        mediaView.setFitHeight(576);
        //mediaView.setSmooth(true);
        //mediaView.setLayoutX(200);
        //mediaView.setLayoutY(200);
        // Create the DropShadow effect
        /*DropShadow dropshadow = new DropShadow();
        dropshadow.setOffsetY(5.0);
        dropshadow.setOffsetX(5.0);
        dropshadow.setColor(Color.RED);

        mediaView.setEffect(dropshadow);

        Rectangle rect4 = new Rectangle(35, 55, 95, 25);
        rect4.setFill(Color.RED);
        rect4.setStroke(Color.BLACK);
        rect4.setStrokeWidth(1);*/

        // Create the HBox
        // HBox controlBox = new HBox(5, null, null);

        // Create the VBox
        VBox root = new VBox(0, mediaView);

        GridPane gridpane = new GridPane();
        //gridpane.setPadding(new Insets(95));
        //gridpane.setHgap(1);
        //gridpane.setVgap(10);
        //GridPane.setHalignment(rect4, HPos.CENTER);

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
    }
}
