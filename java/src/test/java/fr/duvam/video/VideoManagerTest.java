package fr.duvam.video;

import java.io.PrintStream;
import java.util.concurrent.TimeUnit;

import org.junit.Test;

/**
 * A Sample class used to show OmxplayerProcess capabilities
 */
public class VideoManagerTest {

	
	@Test
	public void playingVideo() throws InterruptedException {
		VideoManager videoManger = new VideoManager();
		String base = "/home/david/Nextcloud/robot/videos/base.mp4";
		String smile = "/home/david/Nextcloud/robot/videos/smile.mp4";
		
		videoManger.play(smile);
		
		PrintStream mPlayer = videoManger.getMplayerIn();
		TimeUnit.SECONDS.sleep(2);
		mPlayer.print("pause");
		
		

		mPlayer.print("loadfile "+smile+" 0");
		mPlayer.print("\n");
		mPlayer.flush();
		
		TimeUnit.SECONDS.sleep(20);
		
	}


}
