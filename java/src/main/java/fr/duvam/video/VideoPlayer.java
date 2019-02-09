
package fr.duvam.video;

import fr.duvam.arduino.Communicator;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class VideoPlayer extends Application {

	Communicator communicator;
	PlayerManager playerManager;
	
	public static void main(String[] args) throws Exception {
		launch(args);
	}

	@Override
	public void start(final Stage stage) throws Exception {

		playerManager = new PlayerManager(stage);
		KeyListener listener = new KeyListener(playerManager);
		playerManager.setKeyListener(listener);
		communicator = new Communicator(listener);

		playerManager.getPlayer(MediaManager.KEY_VIDEO_BASE);

		stage.initStyle(StageStyle.UNDECORATED);
		stage.show();

		initArduinoListener(listener);

	}
	@Override
	public void stop() throws Exception {
		super.stop();
		communicator.disconnect();
		
	}
	
	private void initArduinoListener(KeyListener listener) {
		Thread thread = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					Thread.sleep(10000);
					//PlayingVideoListener playingVideoListener = new PlayingVideoListener(layout);
					while (true) {
						Thread.sleep(100);
						// UI update is run on the Application thread
						Platform.runLater(listener);
						//start base video if stopped
						
					}
				} catch (InterruptedException ex) {
				}
			}
		});
		thread.setDaemon(true);
		thread.start();
	}
	
}