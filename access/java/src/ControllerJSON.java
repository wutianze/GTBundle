public class ControllerJSON{
/*
 * command id, see Status as an example, 0: stop & wait; 1: start report; 2: control command
 */
	private String id;
	private long timestamp;
private double delay;
private String content;
public String getId(){
return id;
}
public void setId(String id){
this.id = id;
}
public long getTimestamp(){
return timestamp;
}
public void setTimestamp(long timestamp){
this.timestamp = timestamp;
}
public double getDelay(){
return delay;
}
public void setDelay(double delay){
this.delay = delay;
}
public String getContent(){
return content;
}
public void setContent(String content){
this.content = content;
}
}
