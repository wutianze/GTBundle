public class ControllerJSON{
/*
 * command id, see Status as an example, 0: stop & wait; 1: start report; 2: control command
 */
	private int id;
private double delay;
private String content;
public int getId(){
return id;
}
public void setId(int id){
this.id = id;
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
