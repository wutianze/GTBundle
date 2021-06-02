import java.net.*;
import java.io.*;
import com.alibaba.fastjson.JSON;

public class Controller
{
	public static void main(String [] args)
	{
		int THRESHOLDCLOSE = 5;
		int THRESHOLDFAR = 5;
		String serverName = args[0];
		int port = Integer.parseInt(args[1]);
		StringBuffer id=new StringBuffer(20);
		Status sharedStatus = new Status(0);
		int count = Integer.parseInt(args[2]);
		int interval = Integer.parseInt(args[3]);
		JavaAccess jA = new JavaAccess(serverName,port);
		try
		{
			Thread readerT = jA.createReader((String msg,Status sS,StringBuffer id_)->{
				System.out.println("receive generator msg:"+msg);
				GeneratorJSON gj = JSON.parseObject(msg,GeneratorJSON.class);
				if(gj == null){
					System.out.println("receive generator msg null, may because receive matched msg");
					return;
				}
				if(id_.toString().equals("")){
				id_.append(gj.getId());
				}
				Log updatedLog = JSON.parseObject(gj.getLog(),Log.class);
				if(updatedLog.getCpuAllocated()-updatedLog.getCpuUsage()<THRESHOLDCLOSE){
					sS.set(1);//start control
					System.out.println("cpu usage is close to cpu allocated, control trigger");
				}else if(updatedLog.getCpuAllocated()-updatedLog.getCpuUsage()>THRESHOLDFAR){
				sS.set(2);//start control
					System.out.println("cpu usage << cpu allocated, control trigger");

				}else{
				sS.set(0);
					System.out.println("perfectly match, no action needed");
				}
			},sharedStatus,id);
			//long currentTime=System.currentTimeMillis();
			//start reporting
			//jA.send("{\"id\":\"id0\",\"timestamp\":"+ String.valueOf(currentTime) +",\"delay\":-1,\"content\":\"report\"}");
			int countNow = 0;
			while(countNow<count){
				//currentTime=System.currentTimeMillis();
				if(sharedStatus.check()==1){
					jA.send("{\"id\":\""+id+"\",\"command\":{\"cpuAllocated\":100}}");
					sharedStatus.set(0);
				}else if(sharedStatus.check()==2){
				jA.send("{\"id\":\""+id+"\",\"command\":{\"cpuAllocated\":50}}");
					sharedStatus.set(0);
				}
				countNow++;
				Thread.sleep(interval);	
			}
			jA.send("exit");
			jA.disconnect();
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
