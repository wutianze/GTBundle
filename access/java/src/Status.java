class Status
{
	/* status:
0: wait, do nothing
1: cpu_usage > cpu_allocated
2: cpu_usage < cpu_allocated
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
