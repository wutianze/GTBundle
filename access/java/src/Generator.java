import java.net.*;
import java.io.*;
import com.alibaba.fastjson.JSON;
public class Generator
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
System.out.println("receive controller msg:"+msg);
ControllerJSON cj = JSON.parseObject(msg,ControllerJSON.class);
if(sS == null){
System.out.println("sS null"+msg);
}
if(cj == null){
System.out.println("cj null"+msg);
}
if((cj.getContent()).equals("report"))
{
	sS.set(1);
}else if((cj.getContent()).equals("control")){
	sS.set(2);
}
},sharedStatus);
int countNow = 0;
while(countNow<count){
	long currentTime=System.currentTimeMillis();
	if(sharedStatus.check() == 1){
jA.send("{\"id\":\"id0\",\"seq\":"+String.valueOf(countNow)+",\"timestamp\":"+ String.valueOf(currentTime) +",\"delay\":10.0,\"content\":\"testing\",\"status\":"+String.valueOf(sharedStatus.check())+"}");
//jA.send("{\"msgType\":\"cloud_platform_info_total\",\"msgId\":\"1\",\"msgTag\":{\"stationName\":\"盐城机房测试\"},\"content\":{\"disk_total\":\"0TB\",\"ram_total\":\"1TB\",\"net_band\":\"100G\",\"cpu_total\":\"0\"}}");
	}else if(sharedStatus.check() == 2){
jA.send("{\"id\":\"id0\",\"seq\":"+String.valueOf(countNow)+",\"timestamp\":"+ String.valueOf(currentTime) +",\"delay\":10.0,\"content\":\"testing\",\"status\":"+String.valueOf(sharedStatus.check())+"}");
//jA.send("{\"msgType\":\"cloud_platform_info_total\",\"msgId\":\"1\",\"msgTag\":{\"stationName\":\"盐城机房测试\"},\"content\":{\"disk_total\":\"0TB\",\"ram_total\":\"1TB\",\"net_band\":\"100G\",\"cpu_total\":\"0\"}}");
sharedStatus.set(1);
	}
	countNow++;
Thread.sleep(interval);	
}
//jA.send("exit");
//jA.disconnect();
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
