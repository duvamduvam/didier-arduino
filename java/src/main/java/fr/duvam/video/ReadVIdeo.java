package fr.duvam.video;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintStream;

import org.apache.log4j.Logger;

import fr.duvam.command.Command;

/**
 * A Sample class used to show OmxplayerProcess capabilities
 */
public class ReadVIdeo {

	private static final Logger LOGGER = Logger.getLogger(ReadVIdeo.class);	
	
	
	public String playingVideo() {
		Command command = new Command();
		return command.playingVideo();
	}
	
	public void play(String path) {

		try {
			Process mplayerProcess = Runtime.getRuntime()
					.exec("mplayer -fs /home/david/Nextcloud/robot/videos/base.mp4");

			// create the piped streams where to redirect the standard output and error of
			// MPlayer
			// specify a bigger pipesize than the default of 1024
			PipedInputStream readFrom = new PipedInputStream(600 * 800);
			PipedOutputStream writeTo;

			writeTo = new PipedOutputStream(readFrom);

			BufferedReader mplayerOutErr = new BufferedReader(new InputStreamReader(readFrom));

			// create the threads to redirect the standard output and error of MPlayer
			new LineRedirecter(mplayerProcess.getInputStream(), writeTo).start();
			new LineRedirecter(mplayerProcess.getErrorStream(), writeTo).start();

			// the standard input of MPlayer
			PrintStream mplayerIn = new PrintStream(mplayerProcess.getOutputStream());
		} catch (IOException e) {
			LOGGER.error(e);
		}

	}

}
