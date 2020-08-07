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
         JavaAccess jA = new JavaAccess(serverName,port);
Thread readerT = jA.createReader((String msg)->{
System.out.println("just print msg:"+msg);
});
jA.send("{\"clientIP\":\"aaa\",\"seq\":10}");
Thread.sleep(3000);	
jA.send("{\"clientIP\":\"aaa\",\"seq\":10}");
Thread.sleep(3000);	
//jA.send("exit");
//jA.disconnect();
readerT.join();

System.out.println("java Client stop");
      }catch(Exception e)
      {
         e.printStackTrace();
      }
   }
}
