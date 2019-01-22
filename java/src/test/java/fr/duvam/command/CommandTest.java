package fr.duvam.command;

import java.util.concurrent.TimeUnit;

import org.junit.Test;


/**
 * @author Crunchify.com
 * 
 */

public class CommandTest {
	
	
	
	@Test
	public void testIsPlaying() {
		Command command = new Command();
		String videoName = command.playingVideo();
		System.out.println(videoName);
	}

	@Test
	public void playVIdeoTest() throws InterruptedException {
		Command command = new Command();
		String play ="mplayer /home/david/Nextcloud/robot/videos/base.mp4";
		command.execute(play);
		TimeUnit.SECONDS.sleep(20);
	}
	
}
