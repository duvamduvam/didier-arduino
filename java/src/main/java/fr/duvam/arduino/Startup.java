package fr.duvam.arduino;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Scanner;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

/**
 * Simple application that is part of an tutorial. The tutorial shows how to
 * establish a serial connection between a Java and Arduino program.
 * 
 * @author Michael Schoeffler (www.mschoeffler.de)
 *
 */

public class Startup {

	public static void main(String[] args) throws IOException, InterruptedException {
		SerialPort sp = SerialPort.getCommPort("/dev/ttyS4"); // device name TODO: must be changed
		sp.setComPortParameters(9600, 8, 1, 0); // default connection settings for Arduino
		sp.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0); // block until bytes can be written

		if (sp.openPort()) {
			System.out.println("Port is open :)");
		} else {
			System.out.println("Failed to open port :(");
			return;
		}

		/*
		 * for (Integer i = 0; i < 5; ++i) { sp.getOutputStream().write(i.byteValue());
		 * sp.getOutputStream().flush(); System.out.println("Sent number: " + i);
		 * Thread.sleep(1000); }
		 */

		sp.addDataListener(new SerialPortDataListener() {

			@Override
			public int getListeningEvents() {
				return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
			}

			@Override
			public void serialEvent(SerialPortEvent event) {

				/*
				 * byte[] newData = event.getReceivedData();
				 * System.out.println("Received data of size: " + newData.length); for (int i =
				 * 0; i < newData.length; ++i) System.out.print((char) newData[i]);
				 * System.out.println("\n");
				 */

				// if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
				// return;
				byte[] newData = new byte[sp.bytesAvailable()];
				// byte[] newData2 = event.getReceivedData();

				Scanner in = new Scanner(sp.getInputStream());
				while (sp.bytesAvailable() > 0) {
					if (in.hasNext()) {
						System.out.println(in.nextLine());
					}
				}

				// int numRead = sp.readBytes(newData, newData.length);
				// System.out.println("Read " + numRead + " bytes.");
				// System.out.println(newData[1]);

				// byte[] newData2 = event.getReceivedData();
				// System.out.println("Received data of size: " + newData2.length);
				// for (int i = 0; i < newData2.length; ++i)
				// System.out.print((char) newData2[i]);
				// System.out.println("\n");*/
			}
		});

		/*
		 * Scanner in; PrintWriter out; String input = "";
		 * 
		 * in = new Scanner(sp.getInputStream()); out = new
		 * PrintWriter(sp.getOutputStream()); sp.addDataListener(new
		 * SerialPortDataListener() {
		 * 
		 * @Override public int getListeningEvents() { return
		 * SerialPort.LISTENING_EVENT_DATA_AVAILABLE; }
		 * 
		 * @Override public void serialEvent(SerialPortEvent serialPortEvent) { String
		 * input = ""; boolean received = false; while (sp.bytesAvailable() > 0) input =
		 * in.nextLine();
		 * 
		 * System.out.println("return: " + input); received = true; } });
		 */

		/*
		 * try { while (true) { while (sp.bytesAvailable() == 0) Thread.sleep(20);
		 * 
		 * byte[] readBuffer = new byte[sp.bytesAvailable()]; int numRead =
		 * sp.readBytes(readBuffer, readBuffer.length); System.out.println("Read " +
		 * numRead + " bytes.");
		 * 
		 * BufferedReader input = new BufferedReader(new
		 * InputStreamReader(sp.getInputStream()));
		 * System.out.println(input.readLine());
		 * 
		 * } } catch (Exception e) { e.printStackTrace(); } sp.closePort();*>
		 * 
		 * /*if (sp.closePort()) { System.out.println("Port is closed :)"); } else {
		 * System.out.println("Failed to close port :("); return; }
		 */

	}
}
