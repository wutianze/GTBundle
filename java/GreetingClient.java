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
public static int bytes2Int(byte[] b) {
	    return   b[0] & 0xFF |
	            (b[1] & 0xFF) << 8 |
	            (b[2] & 0xFF) << 16 |
	            (b[3] & 0xFF) << 24;
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
	 byte[] tmp = ("{\"clientIP\":\"aaa\",\"seq\":10}").getBytes("UTF-8");
      	System.out.println("bytes len:"+tmp.length);
         out.write(int2Bytes(tmp.length));
         out.write(tmp);
Thread.sleep(3000);	
	 out.write(int2Bytes(tmp.length));
         out.write(tmp);

         
	 //System.out.println("服务器响应： " + in.read());
    byte[] endd =   "exit".getBytes("UTF-8");  
    out.write(int2Bytes(endd.length));
         out.write(endd);
InputStream inFromServer = client.getInputStream();
	 DataInputStream in = new DataInputStream(inFromServer);
while(true){         
int rec_size=0;
	 byte[] rec_s = new byte[4];
         int len0;
	 if((len0 = in.read(rec_s))!=-1){
	 rec_size = bytes2Int(rec_s);
	 System.out.println("len0:"+len0+" rec_size:"+rec_size);
	 }else{
	 break;
	 }

	 byte[] bytes = new byte[rec_size];
    in.readFully(bytes);
      System.out.println("respond content:"+new String(bytes, 0, rec_size,"UTF-8"));
}
         client.close();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
catch(Exception e)
      {
         e.printStackTrace();
      }
   }
}
