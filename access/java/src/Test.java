import java.net.*;
import java.io.*;
 


public class Test
{

   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      try
      {
         /*System.out.println("连接到主机：" + serverName + " ，端口号：" + port);
         Socket client = new Socket(serverName, port);
         System.out.println("远程主机地址：" + client.getRemoteSocketAddress());
         
	 OutputStream outToServer = client.getOutputStream();
         DataOutputStream out = new DataOutputStream(outToServer);
 
	 InputStream inFromServer = client.getInputStream();
	 DataInputStream in = new DataInputStream(inFromServer);

	 ReceiveClient rThread = new ReceiveClient();
	 rThread.setIn(in);
	 Thread thread = new Thread(rThread);
	 thread.start();

	 byte[] tmp = ("{\"clientIP\":\"aaa\",\"seq\":10}").getBytes("UTF-8");
      	System.out.println("bytes len:"+tmp.length);
         out.write(int2Bytes(tmp.length));
         out.write(tmp);
Thread.sleep(3000);	
	 out.write(int2Bytes(tmp.length));
         out.write(tmp);

Thread.sleep(3000);	
         
    byte[] endd =   "exit".getBytes("UTF-8");  
    out.write(int2Bytes(endd.length));
         out.write(endd);

	          client.close();*/
JavaAccess jA = new JavaAccess(serverName,port);
Thread readerT = jA.createReader((String msg)->{
System.out.println("just print msg:"+msg);
});
jA.send("{\"clientIP\":\"aaa\",\"seq\":10}");
Thread.sleep(3000);	
jA.send("{\"clientIP\":\"aaa\",\"seq\":10}");
Thread.sleep(3000);	
jA.send("exit");
jA.disconnect();
readerT.join();

System.out.println("java Client stop");
      }catch(Exception e)
      {
         e.printStackTrace();
      }
   }
}
