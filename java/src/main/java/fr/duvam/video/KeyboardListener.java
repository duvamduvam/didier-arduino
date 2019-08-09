package fr.duvam.video;

import java.awt.event.KeyListener;

import org.apache.log4j.Logger;

public class KeyboardListener implements KeyListener {

	private static final Logger LOGGER = Logger.getLogger(KeyboardListener.class);

	private CommandListener commandListener;
	
	public KeyboardListener(CommandListener commandListener) {
		super();
		this.commandListener = commandListener;
	}

	@Override
	public void keyTyped(java.awt.event.KeyEvent e) {
		LOGGER.info(e);

	}

	@Override
	public void keyPressed(java.awt.event.KeyEvent e) {
		LOGGER.info(e);
	}

	@Override
	public void keyReleased(java.awt.event.KeyEvent e) {
		LOGGER.info(e);
		commandListener.addKey(String.valueOf(e.getKeyChar()));
	}

}
