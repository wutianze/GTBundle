class Status
{
	/* status id:
0: wait, do nothing
1: reporting
2: temp status, control msg send/receive
*/
	private int i;
	public Status(int i){
	this.i = i;
	}
	public synchronized int check(){
	return this.i;
	}
	public synchronized void set(int ii){
	this.i = ii;
	return;
	}
}
