import java.net.*;
import java.io.*;
import com.alibaba.fastjson.JSON;
public class Generator
{
   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      String id="";
      Status sharedStatus = new Status(0);
      int count = Integer.parseInt(args[2]);
      int interval = Integer.parseInt(args[3]);
         JavaAccess jA = new JavaAccess(serverName,port);
      try
      {
Thread readerT = jA.createReader((String msg,Status sS,String id_)->{
System.out.println("receive controller msg:"+msg);
ControllerJSON cj = JSON.parseObject(msg,ControllerJSON.class);
if(cj == null){
System.out.println("cj null"+msg+" ,may receive matched msg");
return;
}
System.out.println(cj.getCommand());
/*
if((cj.getCommand()).equals("report"))
{
	sS.set(1);
}else if((cj.getCommand()).equals("control")){
	sS.set(2);
}
*/
},sharedStatus,id);
int countNow = 0;
sharedStatus.set(1);
while(countNow<count){
	long currentTime=System.currentTimeMillis();
	if(sharedStatus.check() == 1){
jA.send("{\"id\":\"id0\",\"log\":{\"cpuUsage\":0,\"cpuAllocated\":200}}");
	}else if(sharedStatus.check() == 2){
jA.send("{\"id\":\"id0\",\"log\":{\"cpuUsage\":200,\"cpuAllocated\":100}}");
	}
	countNow++;
Thread.sleep(interval);	
}
jA.send("exit");
jA.disconnect();
readerT.join();

System.out.println("java Client in Generator side stop");
      }catch(Exception e)
      {
         e.printStackTrace();
      }finally{
      jA.disconnect();
      }
   }
}
