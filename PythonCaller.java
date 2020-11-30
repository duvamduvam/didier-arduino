import java.io.*;    
public class PythonCaller {
//https://stackoverflow.com/questions/36143043/java-run-python-script-and-monitor-continuous-output
    public static void main(String[] args) throws IOException {
        // set up the command and parameter
        String pythonScriptPath = "/home/pi/test.py";
        String[] cmd = { "python", pythonScriptPath };

        // create runtime to execute external command
        ProcessBuilder pb = new ProcessBuilder(cmd);

        // retrieve output from python script       
        pb.redirectError();

		Process p = pb.start();
        while(true){
            
            System.out.println("Process Started...");
            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
            int ret = new Integer(in.readLine()).intValue();
            System.out.println("value is : "+ret);
        }
    }
}