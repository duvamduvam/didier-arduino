package fr.duvam.command;

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
	
}
