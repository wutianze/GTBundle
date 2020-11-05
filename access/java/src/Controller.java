import java.net.*;
import java.io.*;
import com.alibaba.fastjson.JSON;

public class Controller
{

   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      Status sharedStatus = new Status(0);
      int count = Integer.parseInt(args[2]);
      int interval = Integer.parseInt(args[3]);
         JavaAccess jA = new JavaAccess(serverName,port);
      try
      {
Thread readerT = jA.createReader((String msg,Status sS)->{
System.out.println("receive generator msg:"+msg);
GeneratorJSON gj = JSON.parseObject(msg,GeneratorJSON.class);
if(gj.getDelay()>10){
sS.set(2);//start control
System.out.println("control trigger");
}
},sharedStatus);
//start reporting
jA.send("{\"id\":1,\"delay\":10,\"content\":\"testing\"}");
int countNow = 0;
while(countNow<count){
	if(sharedStatus.check()==2){
System.out.println("send control msg");
jA.send("{\"id\":2,\"delay\":10,\"content\":\"testing\"}");
sharedStatus.set(1);
	}
	countNow++;
Thread.sleep(interval);	
}
//jA.send("exit");
//jA.disconnect();
readerT.join();

System.out.println("java Client in Controller side stop");
      }catch(Exception e)
      {
         e.printStackTrace();
      }finally{
      jA.disconnect();
      }
   }
}
