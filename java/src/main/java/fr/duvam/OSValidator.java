package fr.duvam;

import org.apache.log4j.Logger;

import fr.duvam.arduino.ArduinoComm;

public class OSValidator {

	private static final Logger LOGGER = Logger.getLogger(ArduinoComm.class);

	private static String OS = System.getProperty("os.name", "generic").toLowerCase();

	public static void main(String[] args) {

		System.out.println(OS);

		if (isWindows()) {
			System.out.println("This is Windows");
		} else if (isMac()) {
			System.out.println("This is Mac");
		} else if (isUnix()) {
			System.out.println("This is Unix or Linux");
		} else if (isSolaris()) {
			System.out.println("This is Solaris");
		} else {
			System.out.println("Your OS is not support!!");
		}
	}

	public static boolean isWindows() {
		return OS.contains("win");
	}

	public static boolean isMac() {
		return OS.contains("mac");
	}

	public static boolean isUnix() {
		return (OS.contains("nix") || OS.contains("nux") || OS.contains("aix"));
	}

	public static boolean isSolaris() {
		return OS.contains("sunos");
	}

	public static String getOS() {
		if (isWindows()) {
			return "win";
		} else if (isMac()) {
			return "osx";
		} else if (isUnix()) {
			return "uni";
		} else if (isSolaris()) {
			return "sol";
		} else {
			return "err";
		}
	}

	public static String getFullOS() {
		LOGGER.info("os.name " + System.getProperty("os.name"));
		LOGGER.info("os.version " + System.getProperty("os.version"));
		LOGGER.info("os.arch " + System.getProperty("os.arch"));
		LOGGER.info("unknown " + System.getProperty("unknown"));
		LOGGER.info("generic " + System.getProperty("generic"));
		return OS + "-" + System.getProperty("os.name");
	}

}