import java.net.*;
import java.io.*;
 

public class GreetingClient
{
public static byte[] int2Bytes(int n)
{
    byte[] b = new byte[4];  
  b[0] = (byte) (n & 0xff);  
  b[1] = (byte) (n >> 8 & 0xff);  
  b[2] = (byte) (n >> 16 & 0xff);  
  b[3] = (byte) (n >> 24 & 0xff);  
  return b;  
}
   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      try
      {
         System.out.println("连接到主机：" + serverName + " ，端口号：" + port);
         Socket client = new Socket(serverName, port);
         System.out.println("远程主机地址：" + client.getRemoteSocketAddress());
         OutputStream outToServer = client.getOutputStream();
         DataOutputStream out = new DataOutputStream(outToServer);
 
         //out.write("aaa".getBytes("UTF-8"));
	 String filled = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
	 byte[] tmp = ("{\"clientIP\":\"aaa\",\"seq\":10}").getBytes("UTF-8");
      	System.out.println("bytes len:"+tmp.length);
         out.write(int2Bytes(tmp.length));
         out.write(tmp);
	
	 out.write(int2Bytes(tmp.length));
         out.write(tmp);

         InputStream inFromServer = client.getInputStream();
         DataInputStream in = new DataInputStream(inFromServer);
         in.read();
	 byte[] bytes = new byte[1024];
    	int len;
    if((len = in.read(bytes)) != -1) {
      System.out.println("respond len:"+len+", content:"+new String(bytes, 0, len,"UTF-8"));
    }

	 //System.out.println("服务器响应： " + in.read());
    byte[] endd =   "exit".getBytes("UTF-8");  
    out.write(int2Bytes(endd.length));
         out.write(endd);
         client.close();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}
