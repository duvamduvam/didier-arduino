package fr.duvam.command;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;


/**
 * @author Crunchify.com
 * 
 */

public class Command {
	
	
	private static String PLAYINGVIDEO = "lsof -c mplayer | fgrep \".mkv\" | awk -F\"/\" '{ print $NF; }' | cut -d'.' -f1";
	
	private static final Logger LOGGER = Logger.getLogger(Command.class);
	
	public String playingVideo() {
		return execute(PLAYINGVIDEO);
	}
	

	private String execute(String cmd){
		// you need a shell to execute a command pipeline
		List<String> commands = new ArrayList<String>();

		commands.add("/bin/sh");
		commands.add("-c");
		commands.add(cmd);

		SystemCommandExecutor commandExecutor = new SystemCommandExecutor(commands);
		try {
			int result = commandExecutor.executeCommand();
		} catch (IOException | InterruptedException e) {
			LOGGER.error(e);
		}

		StringBuilder stdout = commandExecutor.getStandardOutputFromCommand();
		StringBuilder stderr = commandExecutor.getStandardErrorFromCommand();

		return stdout.toString();
		
		/*System.out.println("STDOUT");
		System.out.println(stdout);

		System.out.println("STDERR");
		System.out.println(stderr);*/
	}

}
