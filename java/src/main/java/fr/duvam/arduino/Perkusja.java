package fr.duvam.arduino;

import com.fazecast.jSerialComm.SerialPort;

public class Perkusja {

	static public void main(String[] args) {
		SerialPort comPort = SerialPort.getCommPort("/dev/ttyS4"); // device name TODO: must be changed
		comPort.setComPortParameters(9600, 8, 1, 0); // default connection settings for Arduino
		comPort.openPort();
		PacketListener listener = new PacketListener();
		comPort.addDataListener(listener);
		try {
			Thread.sleep(5000);
		} catch (Exception e) {
			e.printStackTrace();
		}
		comPort.removeDataListener();
		comPort.closePort();
	}

}
