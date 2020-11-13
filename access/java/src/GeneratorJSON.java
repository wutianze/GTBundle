public class GeneratorJSON{
private String id;
private long seq;
private long timestamp;
private double delay;
private String content;
private int status;
public long getSeq(){
return seq;
}
public void setSeq(long seq){
this.seq = seq;
}
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
public int getStatus(){
return status;
}
public void setStatus(int status){
this.status = status;
}
}
