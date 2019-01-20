package org.ardulink;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;

public class ReadBuffer {

	private long timeStamp;
	private File file;

	private boolean isFileUpdated( File file ) {
	  this.file = file;
	  this.timeStamp = file.lastModified();

	  if( this.timeStamp != timeStamp ) {
	    this.timeStamp = timeStamp;
	    //Yes, file is updated
	    return true;
	  }
	  //No, file is not updated
	  return false;
	}
	
    public static void main(String[] args) throws IOException, InterruptedException {
        WatchService watchService
          = FileSystems.getDefault().newWatchService();
 
        Path path = Paths.get("/home/david/Nextcloud/robot/python");
 
        path.register(
          watchService, 
            StandardWatchEventKinds.ENTRY_CREATE, 
              StandardWatchEventKinds.ENTRY_DELETE, 
                StandardWatchEventKinds.ENTRY_MODIFY);
 
        WatchKey key;
        while ((key = watchService.take()) != null) {
            for (WatchEvent<?> event : key.pollEvents()) {
                System.out.println(
                  "Event kind:" + event.kind() 
                    + ". File affected: " + event.context() + ".");
            }
            key.reset();
        }
    }
	
	
}
