package fr.duvam.test;

import org.junit.Test;

import uk.co.caprica.vlcj.component.AudioMediaPlayerComponent;

public class JUnit {

	@Test
	public void testGetSoundLenght() {
		
		
		/*JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocation(100, 100);
		frame.setSize(1200, 800);
		frame.setVisible(true);
		//frame.startPlaying();
		
		EmbeddedMediaPlayerComponent mediaPlayerComponent = new EmbeddedMediaPlayerComponent() {
			@Override
			protected FullScreenStrategy onGetFullScreenStrategy() {
				return new XFullScreenStrategy(frame);
			}
		};

		EmbeddedMediaPlayer mediaPlayer = mediaPlayerComponent.getMediaPlayer();
		
		mediaPlayer.playMedia(music);*/
		
		String music = "/home/david/Nextcloud/InstantUpload/Son/artiste.wav";
		
		AudioMediaPlayerComponent mediaPlayerComponent = new AudioMediaPlayerComponent(); // <--- 1
		mediaPlayerComponent.getMediaPlayer().playMedia(music); // <--- 2
		
		//mediaPlayerComponent.
		
	    /*try {
	    	
	        AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(music));
	        Clip clip = AudioSystem.getClip();
	        clip.open(audioInputStream);
	        clip.start();
	        clip.loop(Clip.LOOP_CONTINUOUSLY);
	    } catch (Exception ex) {
	        ex.printStackTrace();
	    }*/
		
	}
	
	
}
