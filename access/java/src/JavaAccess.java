import java.net.*;
import java.io.*;

interface ReaderRun{
void onMessage(String msg, Status sS);
}
class ReceiveClient implements Runnable{
private JavaAccess jA;
private ReaderRun rrun;
public Status sharedStatus;
public void setJa(JavaAccess j){
this.jA = j;
}
public void setRrun(ReaderRun rr){
this.rrun = rr;
}
public void setStatus(Status sS){
this.sharedStatus = sS;
}
public void run(){
while(true){         
	try{
int rec_size=0;
	 byte[] rec_s = new byte[4];
         int len0;
	 if((len0 = this.jA.in.read(rec_s))!=-1){
	 rec_size = JavaAccess.bytes2Int(rec_s);
	 System.out.println("len0:"+len0+" rec_size:"+rec_size);
	 }else{
	 break;
	 }

	 byte[] bytes = new byte[rec_size];
    this.jA.in.readFully(bytes);
    String content = new String(bytes, 0, rec_size,"UTF-8");
      System.out.println("respond content:"+content);
	this.rrun.onMessage(content,this.sharedStatus);
}
catch(Exception e)
      {
         e.printStackTrace();
      System.out.println("reader exception,try reconnecting");
if(this.jA.reconnect(jA.serverName,jA.port)){
      System.out.println("reconnecting success");
      continue;
}else{
      System.out.println("reconnecting fail, exit");
	 return;
}
      }
	}
}
}
public class JavaAccess{
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
private Socket conn;
public DataOutputStream out;
public DataInputStream in;
public String serverName;
public int port;
public JavaAccess(String sN, int p){
	try{
		serverName = sN;
		port = p;
	System.out.println("connect to server:" + serverName + ", port:" + port);
         conn = new Socket(serverName, port);
         System.out.println("remote address:" + conn.getRemoteSocketAddress());
         
         out = new DataOutputStream(conn.getOutputStream());
	 in = new DataInputStream(conn.getInputStream());
	 }catch(Exception e){
	 e.printStackTrace();
	 }
}
public boolean reconnect(String sN,int p){
	try{
		conn.close();
	System.out.println("reconnect to server:" + sN + ", port:" + p);
         conn = new Socket(sN, p);
         System.out.println("remote address:" + conn.getRemoteSocketAddress());
         
         out = new DataOutputStream(conn.getOutputStream());
	 in = new DataInputStream(conn.getInputStream());
	 return true;
}catch(Exception e){
e.printStackTrace();
return false;
}
}
public boolean disconnect(){
	try{
conn.close();
	 return true;
}catch(Exception e){
e.printStackTrace();
return false;
}
}
public boolean send(String jsonString){
	try{
	byte[] tmp = jsonString.getBytes("UTF-8");
      	System.out.println("send bytes len:"+tmp.length);
         out.write(int2Bytes(tmp.length));
         out.write(tmp);
	 return true;
}catch(Exception e){
e.printStackTrace();
return false;
}
}

public Thread createReader(ReaderRun rR,Status s){
ReceiveClient rThread = new ReceiveClient();
rThread.setJa(this);	 
rThread.setRrun(rR);
	 Thread thread = new Thread(rThread);
	 thread.start();
	 System.out.println("new Reader created");
	 return thread;
}
}
