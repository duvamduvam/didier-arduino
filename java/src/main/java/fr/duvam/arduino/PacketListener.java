package fr.duvam.arduino;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.fazecast.jSerialComm.SerialPortPacketListener;

public class PacketListener implements SerialPortPacketListener {
	@Override
	public int getListeningEvents() {
		return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
	}

	@Override
	public int getPacketSize() {
		return 100;
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		byte[] newData = event.getReceivedData();
		System.out.println("Received data of size: " + newData.length);
		for (int i = 0; i < newData.length; ++i)
			System.out.print((char) newData[i]);
		System.out.println("\n");
	}
	static public void main(String[] args)
	{
	   SerialPort comPort = SerialPort.getCommPorts()[0];
	   comPort.openPort();
	   PacketListener listener = new PacketListener();
	   comPort.addDataListener(listener);
	   try { Thread.sleep(5000); } catch (Exception e) { e.printStackTrace(); }
	   comPort.removeDataListener();
	   comPort.closePort();
	}
}


