package fr.duvam;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintStream;

/**
 * A Sample class used to show OmxplayerProcess capabilities
 */
public class Sample {

    public static void main(String[] args) throws IOException {

		Process mplayerProcess = Runtime.getRuntime()
				.exec("mplayer /home/david/Nextcloud/robot/videos/smile.mp4");
    	
    	// create the piped streams where to redirect the standard output and error of MPlayer
    	// specify a bigger pipesize than the default of 1024
    	PipedInputStream  readFrom = new PipedInputStream(256*1024);
    	PipedOutputStream writeTo = new PipedOutputStream(readFrom);
    	BufferedReader mplayerOutErr = new BufferedReader(new InputStreamReader(readFrom));
    	 
    	// create the threads to redirect the standard output and error of MPlayer
    	new LineRedirecter(mplayerProcess.getInputStream(), writeTo).start();
    	new LineRedirecter(mplayerProcess.getErrorStream(), writeTo).start();
    	 
    	// the standard input of MPlayer
    	PrintStream mplayerIn = new PrintStream(mplayerProcess.getOutputStream());
    }

}
